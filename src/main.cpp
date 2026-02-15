#include "Core.hpp"
#include "UI.hpp"
#include <iostream>
#include <csignal>

namespace PayNix::Core {
    std::atomic<bool> g_SystemRunning{true};
}

// OS Signal Trap Gate
void signalHandler(int signum) {
    std::cerr << "\n\n[OS INTERRUPT DETECTED (Signal " << signum << ")]\n";
    std::cerr << "Flushing buffers and locking vaults gracefully...\n";
    PayNix::Core::g_SystemRunning = false;
    PayNix::Core::Logger::getInstance().log(PayNix::Core::LogLevel::SECURITY_ALERT, "System", "Emergency OS Shutdown Intercepted. Halting.");
    exit(signum);
}

int main() {
    // Bind SIGINT (Ctrl+C) and SIGTERM (OS Kill)
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try {
        PayNix::Core::Logger::getInstance().init();
        LOG_INFO("System", "PayNix Application Startup Initiated.");

        PayNix::UI::AppUI app;
        app.run();

        LOG_INFO("System", "PayNix Application Shutdown Safely.");
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "\n[CRITICAL FAILURE]: Unrecoverable boundary reached.\nDetails: " << e.what() << "\n";
        LOG_ERR("System_Fatal", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        LOG_ERR("System_Fatal", "Unknown Exception Type Raised.");
        return EXIT_FAILURE;
    }
}