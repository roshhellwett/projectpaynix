#pragma once
#include <string>
#include "Logic.hpp"

namespace PayNix::UI {
    class Terminal {
    public:
        static void clearScreen();
        static void setColorRed();
        static void setColorGreen();
        static void setColorCyan();
        static void setColorYellow();
        static void resetColor();
        static void printTitle(const std::string& title);
        static void pause();

        static std::string readString(const std::string& prompt, bool hidden = false);
        static int readInt(const std::string& prompt);
        static Core::Currency readCurrency(const std::string& prompt); // Safe financial parsing
    };

    class AppUI {
        Logic::AuthManager authMgr;
        Logic::InventoryManager invMgr;
        Logic::BillingEngine billEngine;

        void adminPanel();
        void employeeMenu();
        void performBilling(); // Implements the "Blind Biller" UI fix
        void manageInventory();

    public:
        AppUI() : billEngine(invMgr) {}
        void run();
    };
}