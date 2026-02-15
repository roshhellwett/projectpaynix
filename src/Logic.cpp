#include "Logic.hpp"
#include <iostream>
#include <thread>

namespace PayNix::Logic {

    AuthManager::AuthManager() : adminRepo(Core::ADMIN_FILE), empRepo(Core::EMP_FILE) {}

    bool AuthManager::isAdminSetup() { return !adminRepo.loadAll().empty(); }

    bool AuthManager::setupAdmin(const std::string& id, const std::string& rawPassword) {
        if (isAdminSetup()) return false;
        Models::Admin a;
        a.adminId = id;
        a.passwordHash = Security::hashPassword(rawPassword);
        LOG_SEC("Auth", "Admin Vault Initialized: " + id);
        return adminRepo.saveAll({a});
    }

    bool AuthManager::verifyAdmin(const std::string& id, const std::string& rawPassword) {
        if (failedAttempts > 0) std::this_thread::sleep_for(std::chrono::seconds(failedAttempts * 2)); // Brute Force Lockout Gate

        auto admins = adminRepo.loadAll();
        if (!admins.empty() && admins[0].adminId.toString() == id && admins[0].passwordHash == Security::hashPassword(rawPassword)) {
            failedAttempts = 0;
            LOG_SEC("Auth", "Admin Authenticated.");
            return true;
        }
        failedAttempts++;
        LOG_SEC("Auth", "Failed Admin Login. Attempt: " + std::to_string(failedAttempts));
        return false;
    }

    bool AuthManager::addEmployee(const std::string& id, const std::string& name, const std::string& dob, const std::string& rawPassword) {
        auto emps = empRepo.loadAll();
        for (const auto& e : emps) if (e.empId.toString() == id) return false;
        Models::Employee e; e.empId = id; e.name = name; e.dob = dob; e.passwordHash = Security::hashPassword(rawPassword);
        emps.push_back(e);
        LOG_INFO("Auth", "Employee provisioned: " + id);
        return empRepo.saveAll(emps);
    }

    std::vector<Models::Employee> AuthManager::getEmployees() { return empRepo.loadAll(); }

    bool AuthManager::loginEmployee(const std::string& id, const std::string& rawPassword) {
        if (failedAttempts > 0) std::this_thread::sleep_for(std::chrono::seconds(failedAttempts * 2));
        
        auto emps = empRepo.loadAll();
        uint64_t hash = Security::hashPassword(rawPassword);
        for (const auto& e : emps) {
            if (e.empId.toString() == id && e.passwordHash == hash) {
                failedAttempts = 0;
                currentSession = e;
                LOG_INFO("Auth", "Session active: " + id);
                return true;
            }
        }
        failedAttempts++;
        LOG_SEC("Auth", "Failed Employee Login.");
        return false;
    }

    void AuthManager::logoutEmployee() {
        if (currentSession) { LOG_INFO("Auth", "Session closed: " + currentSession->empId.toString()); currentSession.reset(); }
    }

    std::optional<Models::Employee> AuthManager::getSession() const { return currentSession; }

    // --- INVENTORY ---
    InventoryManager::InventoryManager() : itemRepo(Core::ITEMS_FILE) {}

    bool InventoryManager::addItem(const std::string& id, const std::string& name, Core::Currency priceCents, uint32_t stock) {
        auto items = itemRepo.loadAll();
        for (const auto& i : items) if (i.id.toString() == id && !i.isDeleted) return false;
        Models::Item it; it.id = id; it.name = name; it.priceCents = priceCents; it.stockQuantity = stock; it.isDeleted = false;
        items.push_back(it);
        LOG_INFO("Inventory", "Stock added: " + id);
        return itemRepo.saveAll(items);
    }

    std::vector<Models::Item> InventoryManager::getActiveItems() {
        auto items = itemRepo.loadAll();
        std::vector<Models::Item> active;
        for (const auto& i : items) if (!i.isDeleted) active.push_back(i);
        return active;
    }

    std::optional<Models::Item> InventoryManager::getItemById(const std::string& id) {
        auto items = getActiveItems();
        for (const auto& i : items) if (i.id.toString() == id) return i;
        return std::nullopt;
    }

    bool InventoryManager::updateItem(const std::string& id, const std::string& newName, Core::Currency newPriceCents, uint32_t addedStock) {
        auto items = itemRepo.loadAll();
        for (auto& i : items) {
            if (i.id.toString() == id && !i.isDeleted) {
                i.name = newName; i.priceCents = newPriceCents; i.stockQuantity += addedStock;
                LOG_INFO("Inventory", "Item modified: " + id);
                return itemRepo.saveAll(items);
            }
        }
        return false;
    }

    bool InventoryManager::deductStock(const std::string& id, uint32_t quantity) {
        auto items = itemRepo.loadAll();
        for (auto& i : items) {
            if (i.id.toString() == id && !i.isDeleted) {
                if (i.stockQuantity < quantity) return false; // Stock Validation Gate
                i.stockQuantity -= quantity;
                return itemRepo.saveAll(items);
            }
        }
        return false;
    }

    bool InventoryManager::deleteItem(const std::string& id) {
        auto items = itemRepo.loadAll();
        for (auto& i : items) {
            if (i.id.toString() == id && !i.isDeleted) {
                i.isDeleted = true;
                LOG_INFO("Inventory", "Item soft-deleted: " + id);
                return itemRepo.saveAll(items);
            }
        }
        return false;
    }

    // --- BILLING ENGINE ---
    BillingEngine::BillingEngine(InventoryManager& im) : billRepo(Core::BILLS_FILE), invManager(im) {}

    std::optional<Models::BillHeader> BillingEngine::createBill(const std::string& custName, const std::string& phone, const std::vector<std::pair<std::string, uint32_t>>& cart) {
        if (cart.empty() || cart.size() > Core::MAX_BILL_ITEMS) return std::nullopt;
        if (!Security::isValidPhone(phone)) return std::nullopt;

        Models::BillHeader bill;
        bill.customerName = custName; bill.phone = phone;
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeBuf[26];
        #ifdef _WIN32
            ctime_s(timeBuf, sizeof(timeBuf), &now);
        #else
            ctime_r(&now, timeBuf);
        #endif
        std::string timeStr(timeBuf);
        if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();
        
        bill.datetime = timeStr;
        bill.billId = "B-" + std::to_string(now);
        bill.itemCount = 0; bill.grandTotalCents = 0;

        // Verify all stock first (Atomicity preparation)
        for (const auto& req : cart) {
            auto itemOpt = invManager.getItemById(req.first);
            if (!itemOpt || itemOpt->stockQuantity < req.second) return std::nullopt; // Fails if ANY item lacks stock
            
            Models::BillItem bi;
            bi.itemId = itemOpt->id; bi.itemName = itemOpt->name; bi.priceCents = itemOpt->priceCents; bi.quantity = req.second;
            bi.totalCents = bi.priceCents * bi.quantity;
            
            if (bill.grandTotalCents + bi.totalCents > Core::MAX_BILL_CENTS) return std::nullopt; // Math Overflow Gate
            
            bill.grandTotalCents += bi.totalCents;
            bill.items[bill.itemCount++] = bi;
        }

        // Execution Phase (Deduct Stock & Save Bill)
        for (uint32_t i = 0; i < bill.itemCount; ++i) {
            invManager.deductStock(bill.items[i].itemId.toString(), bill.items[i].quantity);
        }

        auto bills = billRepo.loadAll();
        bills.push_back(bill);
        if (billRepo.saveAll(bills)) {
            LOG_INFO("Billing", "Transaction Confirmed: " + bill.billId.toString());
            exportBillToJson(bill);
            return bill;
        }
        return std::nullopt;
    }

    std::vector<Models::BillHeader> BillingEngine::getBillsByPhone(const std::string& last4) {
        auto bills = billRepo.loadAll();
        std::vector<Models::BillHeader> results;
        for (const auto& b : bills) {
            std::string phone = b.phone.toString();
            if (phone.length() >= 4 && phone.substr(phone.length() - 4) == last4) results.push_back(b);
        }
        return results;
    }

    void BillingEngine::exportBillToJson(const Models::BillHeader& bill) {
        std::ofstream out(Core::JSON_EXPORT_DIR + bill.billId.toString() + ".json");
        if (!out) return;
        out << "{\n  \"billId\": \"" << bill.billId.toString() << "\",\n  \"customerName\": \"" << bill.customerName.toString() << "\",\n";
        out << "  \"phone\": \"" << bill.phone.toString() << "\",\n  \"datetime\": \"" << bill.datetime.toString() << "\",\n";
        out << "  \"grandTotal\": \"" << Core::formatCurrency(bill.grandTotalCents) << "\",\n  \"items\": [\n";
        for (uint32_t i = 0; i < bill.itemCount; ++i) {
            out << "    { \"name\": \"" << bill.items[i].itemName.toString() << "\", \"qty\": " << bill.items[i].quantity << ", \"total\": \"" << Core::formatCurrency(bill.items[i].totalCents) << "\" }";
            if (i < bill.itemCount - 1) out << ",";
            out << "\n";
        }
        out << "  ]\n}\n";
    }

    // The Z-Report Audit Gate
    bool BillingEngine::generateZReport(const std::string& dateString) {
        auto bills = billRepo.loadAll();
        Core::Currency dailyTotal = 0;
        uint32_t transactionCount = 0;
        
        std::string filename = Core::Z_REPORT_DIR + "Z_Report_" + dateString + ".txt";
        std::ofstream out(filename);
        if(!out) return false;

        out << "=== END OF DAY Z-REPORT: " << dateString << " ===\n\n";
        for (const auto& b : bills) {
            if (b.datetime.toString().find(dateString) != std::string::npos) {
                dailyTotal += b.grandTotalCents;
                transactionCount++;
                out << "[" << b.billId.toString() << "] " << Core::formatCurrency(b.grandTotalCents) << "\n";
            }
        }
        out << "\n--------------------------\n";
        out << "TOTAL TRANSACTIONS: " << transactionCount << "\n";
        out << "GROSS REVENUE: " << Core::formatCurrency(dailyTotal) << "\n";
        out << "==========================\n";
        
        LOG_SEC("Audit", "Z-Report Generated for " + dateString);
        return true;
    }
}