#include "UI.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <cmath>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace PayNix::UI {

    void Terminal::clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void Terminal::setColorRed() { std::cout << "\033[31m"; }
    void Terminal::setColorGreen() { std::cout << "\033[32m"; }
    void Terminal::setColorCyan() { std::cout << "\033[36m"; }
    void Terminal::setColorYellow() { std::cout << "\033[33m"; }
    void Terminal::resetColor() { std::cout << "\033[0m"; }

    void Terminal::printTitle(const std::string& title) {
        setColorCyan(); std::cout << "+--------------------------------------+\n| " << title;
        for(size_t i = title.length(); i < 36; ++i) std::cout << " ";
        std::cout << " |\n+--------------------------------------+\n"; resetColor();
    }

    void Terminal::pause() {
        std::cout << "\nPRESS ENTER TO CONTINUE...";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string Terminal::readString(const std::string& prompt, bool hidden) {
        std::cout << prompt;
        std::string result;
        if (!hidden) {
            std::getline(std::cin, result);
        } else {
            #ifdef _WIN32
                int inputChar;
                while ((inputChar = _getch()) != '\r') {
                    char ch = static_cast<char>(inputChar);
                    if (ch == '\b' && !result.empty()) { result.pop_back(); std::cout << "\b \b"; }
                    else if (ch >= 32 && result.size() < Core::MAX_INPUT_LENGTH) { result.push_back(ch); std::cout << '*'; }
                }
                std::cout << '\n';
            #else
                struct termios oldt, newt; tcgetattr(STDIN_FILENO, &oldt); newt = oldt; newt.c_lflag &= ~(ICANON | ECHO); tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                char ch;
                while (read(STDIN_FILENO, &ch, 1) == 1 && ch != '\n') {
                    if ((ch == 127 || ch == '\b') && !result.empty()) { result.pop_back(); std::cout << "\b \b"; }
                    else if (ch >= 32 && result.size() < Core::MAX_INPUT_LENGTH) { result.push_back(ch); std::cout << '*'; }
                }
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt); std::cout << '\n';
            #endif
        }
        if (std::cin.eof()) exit(1);
        std::cin.clear(); // Flush Buffer Gate
        return Security::sanitizeInput(result);
    }

    int Terminal::readInt(const std::string& prompt) {
        while (true) {
            std::string input = readString(prompt);
            try { if(!input.empty()) return std::stoi(input); } 
            catch (...) { setColorRed(); std::cout << "INVALID NUMBER.\n"; resetColor(); }
        }
    }

    Core::Currency Terminal::readCurrency(const std::string& prompt) {
        while (true) {
            std::string input = readString(prompt);
            try { 
                if(!input.empty()) {
                    float f = std::stof(input);
                    if(f < 0.0f) throw std::invalid_argument("Negative");
                    return static_cast<Core::Currency>(std::round(f * 100.0f)); // Math Safe Parse Gate
                }
            } 
            catch (...) { setColorRed(); std::cout << "INVALID FORMAT (e.g. 10.99).\n"; resetColor(); }
        }
    }

    void AppUI::run() {
        while (Core::g_SystemRunning) {
            Terminal::clearScreen();
            Terminal::printTitle("PAYNIX TERMINAL V3.0 (SECURE)");
            std::cout << "1. EMPLOYEE LOGIN\n2. ADMIN PANEL\n3. EXIT\n\n";
            int choice = Terminal::readInt("ENTER CHOICE: ");
            if (!Core::g_SystemRunning) break;

            try {
                if (choice == 1) {
                    Terminal::clearScreen(); Terminal::printTitle("EMPLOYEE LOGIN");
                    std::string id = Terminal::readString("EMPLOYEE ID: ");
                    std::string pass = Terminal::readString("PASSWORD: ", true);
                    if (authMgr.loginEmployee(id, pass)) employeeMenu();
                    else { Terminal::setColorRed(); std::cout << "ACCESS DENIED.\n"; Terminal::resetColor(); Terminal::pause(); }
                } else if (choice == 2) {
                    adminPanel();
                } else if (choice == 3) {
                    break;
                }
            } catch (const std::exception& e) {
                Terminal::setColorRed(); std::cout << "\nSYSTEM TRAP: " << e.what() << "\n"; Terminal::resetColor(); Terminal::pause();
            }
        }
    }

    void AppUI::adminPanel() {
        if (!authMgr.isAdminSetup()) {
            Terminal::clearScreen(); Terminal::printTitle("VAULT INITIALIZATION");
            std::string id = Terminal::readString("SET MASTER ID: ");
            std::string pass = Terminal::readString("SET PASSPHRASE: ", true);
            if(authMgr.setupAdmin(id, pass)) { Terminal::setColorGreen(); std::cout << "VAULT SECURED.\n"; Terminal::resetColor(); }
            Terminal::pause(); return;
        }

        Terminal::clearScreen(); Terminal::printTitle("ADMIN LOGIN");
        std::string id = Terminal::readString("ADMIN ID: ");
        std::string pass = Terminal::readString("PASSWORD: ", true);
        if (!authMgr.verifyAdmin(id, pass)) { Terminal::setColorRed(); std::cout << "LOCKOUT PROTOCOL ENGAGED.\n"; Terminal::resetColor(); Terminal::pause(); return; }

        while (Core::g_SystemRunning) {
            Terminal::clearScreen(); Terminal::printTitle("SECURE ADMIN PORTAL");
            std::cout << "1. ADD EMPLOYEE\n2. VIEW EMPLOYEES\n3. GENERATE Z-REPORT\n4. LOGOUT\n\n";
            int choice = Terminal::readInt("CHOICE: ");

            if (choice == 1) {
                std::string eId = Terminal::readString("EMP ID: "); std::string eName = Terminal::readString("NAME: ");
                std::string eDob = Terminal::readString("DOB: "); std::string ePass = Terminal::readString("TEMP PASSWORD: ", true);
                if (authMgr.addEmployee(eId, eName, eDob, ePass)) { Terminal::setColorGreen(); std::cout << "ADDED.\n"; Terminal::resetColor(); } 
                else { Terminal::setColorRed(); std::cout << "FAILED.\n"; Terminal::resetColor(); }
                Terminal::pause();
            } else if (choice == 2) {
                auto emps = authMgr.getEmployees();
                for (const auto& e : emps) std::cout << "ID: " << e.empId.toString() << " | NAME: " << e.name.toString() << "\n";
                Terminal::pause();
            } else if (choice == 3) {
                std::string date = Terminal::readString("ENTER DATE (e.g. 16-Feb): ");
                if(billEngine.generateZReport(date)) { Terminal::setColorGreen(); std::cout << "Z-REPORT GENERATED IN reports/ FOLDER.\n"; Terminal::resetColor(); }
                Terminal::pause();
            } else if (choice == 4) break;
        }
    }

    void AppUI::employeeMenu() {
        while (Core::g_SystemRunning) {
            Terminal::clearScreen(); Terminal::printTitle("DASHBOARD - " + authMgr.getSession()->name.toString());
            std::cout << "1. NEW TRANSACTION\n2. INVENTORY MANAGER\n3. LOGOUT\n\n";
            int choice = Terminal::readInt("CHOICE: ");
            if (choice == 1) performBilling();
            else if (choice == 2) manageInventory();
            else if (choice == 3) { authMgr.logoutEmployee(); break; }
        }
    }

    void AppUI::manageInventory() {
        while (Core::g_SystemRunning) {
            Terminal::clearScreen(); Terminal::printTitle("INVENTORY MANAGER");
            std::cout << "1. ADD ITEM\n2. LIST ACTIVE STOCK\n3. UPDATE ITEM / ADD STOCK\n4. DELETE ITEM\n5. BACK\n\n";
            int choice = Terminal::readInt("CHOICE: ");

            if (choice == 1) {
                std::string id = Terminal::readString("ITEM ID: "); std::string name = Terminal::readString("NAME: ");
                Core::Currency price = Terminal::readCurrency("PRICE (e.g. 10.99): ");
                uint32_t stock = Terminal::readInt("INITIAL STOCK QTY: ");
                if (invMgr.addItem(id, name, price, stock)) { Terminal::setColorGreen(); std::cout << "SECURED.\n"; Terminal::resetColor(); }
                Terminal::pause();
            } else if (choice == 2) {
                auto items = invMgr.getActiveItems();
                Terminal::setColorYellow(); std::cout << "ID\t\tNAME\t\tPRICE\t\tSTOCK\n"; Terminal::resetColor();
                for (const auto& i : items) std::cout << i.id.toString() << "\t\t" << i.name.toString() << "\t\t" << Core::formatCurrency(i.priceCents) << "\t\t" << i.stockQuantity << "\n";
                Terminal::pause();
            } else if (choice == 3) {
                std::string id = Terminal::readString("ITEM ID TO UPDATE: "); std::string name = Terminal::readString("NEW NAME: ");
                Core::Currency price = Terminal::readCurrency("NEW PRICE: ");
                uint32_t stock = Terminal::readInt("STOCK TO ADD (0 to keep current): ");
                if (invMgr.updateItem(id, name, price, stock)) { Terminal::setColorGreen(); std::cout << "UPDATED.\n"; Terminal::resetColor(); }
                Terminal::pause();
            } else if (choice == 4) {
                std::string id = Terminal::readString("ITEM ID TO DELETE: ");
                if (invMgr.deleteItem(id)) { Terminal::setColorGreen(); std::cout << "DELETED.\n"; Terminal::resetColor(); }
                Terminal::pause();
            } else if (choice == 5) break;
        }
    }

    // The Live "Blind Biller" Fix & Cart Editing Gate
    void AppUI::performBilling() {
        std::string cName = Terminal::readString("CUSTOMER NAME: ");
        std::string phone = Terminal::readString("PHONE (10 Digits or 0000000000 for Walk-In): ");
        if (!Security::isValidPhone(phone)) { Terminal::setColorRed(); std::cout << "INVALID PHONE REGEX.\n"; Terminal::resetColor(); Terminal::pause(); return; }

        std::vector<std::pair<std::string, uint32_t>> cart;
        
        while (Core::g_SystemRunning) {
            Terminal::clearScreen();
            Terminal::printTitle("ACTIVE TERMINAL");
            
            // 1. Show Active Cart
            Core::Currency liveTotal = 0;
            Terminal::setColorYellow(); std::cout << "--- CURRENT CART ---\n"; Terminal::resetColor();
            if (cart.empty()) std::cout << "Cart is empty.\n";
            else {
                for (size_t i = 0; i < cart.size(); ++i) {
                    auto it = invMgr.getItemById(cart[i].first);
                    if(it) {
                        Core::Currency lineTotal = it->priceCents * cart[i].second;
                        liveTotal += lineTotal;
                        std::cout << "[" << i << "] " << it->name.toString() << " x" << cart[i].second << " (" << Core::formatCurrency(lineTotal) << ")\n";
                    }
                }
            }
            Terminal::setColorGreen(); std::cout << "LIVE TOTAL: " << Core::formatCurrency(liveTotal) << "\n\n"; Terminal::resetColor();

            // 2. Show Live Inventory
            Terminal::setColorCyan(); std::cout << "--- AVAILABLE STOCK ---\n"; Terminal::resetColor();
            auto items = invMgr.getActiveItems();
            for (const auto& i : items) {
                std::cout << i.id.toString() << " | " << i.name.toString() << " | " << Core::formatCurrency(i.priceCents) << " | Stock: " << i.stockQuantity << "\n";
            }

            // 3. Command Prompt
            std::cout << "\nCOMMANDS: [Type ITEM ID] | [DONE] | [CANCEL] | [REMOVE <index>]\n";
            std::string cmd = Terminal::readString("INPUT: ");
            
            if (cmd == "DONE" || cmd == "done") break;
            if (cmd == "CANCEL" || cmd == "cancel") return; // The Abandoned Cart Gate
            
            if (cmd.substr(0, 6) == "REMOVE" || cmd.substr(0, 6) == "remove") { // Cart Edit Gate
                try {
                    size_t idx = std::stoull(cmd.substr(7));
                    if(idx < cart.size()) cart.erase(cart.begin() + idx);
                } catch(...) {}
                continue;
            }

            auto itemOpt = invMgr.getItemById(cmd);
            if (!itemOpt) { Terminal::setColorRed(); std::cout << "INVALID ITEM ID.\n"; Terminal::resetColor(); Terminal::pause(); continue; }
            
            int qty = Terminal::readInt("QUANTITY: ");
            if (qty <= 0 || (uint32_t)qty > itemOpt->stockQuantity) { // Quantity & Stock Trap Gate
                Terminal::setColorRed(); std::cout << "INVALID QUANTITY OR OUT OF STOCK.\n"; Terminal::resetColor(); Terminal::pause(); continue; 
            }
            cart.push_back({cmd, static_cast<uint32_t>(qty)});
        }

        if(cart.empty()) return;

        auto billOpt = billEngine.createBill(cName, phone, cart); // ACID Transaction execution
        if (billOpt) {
            Terminal::clearScreen(); Terminal::printTitle("TRANSACTION SUCCESS");
            std::cout << "BILL ID: " << billOpt->billId.toString() << "\nGRAND TOTAL: " << Core::formatCurrency(billOpt->grandTotalCents) << "\n";
            Terminal::setColorGreen(); std::cout << "STOCK DEDUCTED. BILL SECURED.\n"; Terminal::resetColor();
        } else {
            Terminal::setColorRed(); std::cout << "TRANSACTION FAILED (Sync Error or Stock Depleted).\n"; Terminal::resetColor();
        }
        Terminal::pause();
    }
}