#pragma once
#include "Data.hpp"
#include <memory>
#include <optional>

namespace PayNix::Logic {
    class AuthManager {
        Data::SecureRepository<Models::Admin> adminRepo;
        Data::SecureRepository<Models::Employee> empRepo;
        std::optional<Models::Employee> currentSession;
        uint32_t failedAttempts = 0; // Brute Force Gate

    public:
        AuthManager();
        bool isAdminSetup();
        bool setupAdmin(const std::string& id, const std::string& rawPassword);
        bool verifyAdmin(const std::string& id, const std::string& rawPassword);
        
        bool addEmployee(const std::string& id, const std::string& name, const std::string& dob, const std::string& rawPassword);
        std::vector<Models::Employee> getEmployees();
        
        bool loginEmployee(const std::string& id, const std::string& rawPassword);
        void logoutEmployee();
        std::optional<Models::Employee> getSession() const;
    };

    class InventoryManager {
        Data::SecureRepository<Models::Item> itemRepo;
    public:
        InventoryManager();
        bool addItem(const std::string& id, const std::string& name, Core::Currency priceCents, uint32_t stock);
        bool updateItem(const std::string& id, const std::string& newName, Core::Currency newPriceCents, uint32_t addedStock);
        bool deductStock(const std::string& id, uint32_t quantity); // Reality Gate
        bool deleteItem(const std::string& id); 
        std::vector<Models::Item> getActiveItems();
        std::optional<Models::Item> getItemById(const std::string& id);
    };

    class BillingEngine {
        Data::SecureRepository<Models::BillHeader> billRepo;
        InventoryManager& invManager;
    public:
        BillingEngine(InventoryManager& im);
        std::optional<Models::BillHeader> createBill(const std::string& custName, const std::string& phone, const std::vector<std::pair<std::string, uint32_t>>& cart);
        std::vector<Models::BillHeader> getBillsByPhone(const std::string& last4);
        void exportBillToJson(const Models::BillHeader& bill);
        bool generateZReport(const std::string& dateString); // Audit Gate
    };
}