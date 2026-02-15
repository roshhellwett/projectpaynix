#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <mutex>
#include <ctime>
#include <atomic>
#include <iomanip>
#include <sstream>

namespace PayNix::Core {
    // Application States & Invariants
    constexpr uint32_t APP_VERSION = 30000; // v3.0.0 (Schema Migration Gate)
    constexpr uint32_t FILE_MAGIC = 0x504E4958; // "PNIX"
    constexpr size_t MAX_BILL_ITEMS = 100;
    constexpr size_t MAX_INPUT_LENGTH = 128; // Buffer Overflow protection
    
    // Financial Safety (Integer Cent Math)
    using Currency = uint64_t; 
    constexpr Currency MAX_BILL_CENTS = 1000000000; // $10,000,000.00 max

    extern std::atomic<bool> g_SystemRunning; // SIGINT Trap Gate

    const std::string DATA_DIR = "data/";
    const std::string ADMIN_FILE = DATA_DIR + "admin.dat";
    const std::string EMP_FILE = DATA_DIR + "employees.dat";
    const std::string ITEMS_FILE = DATA_DIR + "items.dat";
    const std::string BILLS_FILE = DATA_DIR + "bills.dat";
    const std::string SYS_LOG_FILE = DATA_DIR + "system_audit.log";
    const std::string JSON_EXPORT_DIR = "exports/";
    const std::string Z_REPORT_DIR = "reports/";

    enum class LogLevel { INFO, WARNING, ERROR, SECURITY_ALERT };

    class Logger {
        std::mutex mtx;
        std::ofstream logFile;
    public:
        static Logger& getInstance() { static Logger instance; return instance; }

        void init() {
            std::filesystem::create_directories(DATA_DIR);
            std::filesystem::create_directories(JSON_EXPORT_DIR);
            std::filesystem::create_directories(Z_REPORT_DIR);
            logFile.open(SYS_LOG_FILE, std::ios::app);
        }

        void log(LogLevel level, const std::string& component, const std::string& message) {
            std::lock_guard<std::mutex> lock(mtx);
            if (!logFile.is_open()) return;

            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            char timeBuf[26];
            #ifdef _WIN32
                ctime_s(timeBuf, sizeof(timeBuf), &now);
            #else
                ctime_r(&now, timeBuf);
            #endif
            std::string timeStr(timeBuf);
            if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();

            std::string lvlStr;
            switch(level) {
                case LogLevel::INFO: lvlStr = "[INFO]"; break;
                case LogLevel::WARNING: lvlStr = "[WARN]"; break;
                case LogLevel::ERROR: lvlStr = "[ERROR]"; break;
                case LogLevel::SECURITY_ALERT: lvlStr = "[SECURITY]"; break;
            }
            logFile << timeStr << " " << lvlStr << " [" << component << "] " << message << "\n";
            logFile.flush();
        }
    };

    inline std::string formatCurrency(Currency cents) {
        std::stringstream ss;
        ss << "$" << (cents / 100) << "." << std::setfill('0') << std::setw(2) << (cents % 100);
        return ss.str();
    }

    #define LOG_INFO(comp, msg) PayNix::Core::Logger::getInstance().log(PayNix::Core::LogLevel::INFO, comp, msg)
    #define LOG_ERR(comp, msg) PayNix::Core::Logger::getInstance().log(PayNix::Core::LogLevel::ERROR, comp, msg)
    #define LOG_SEC(comp, msg) PayNix::Core::Logger::getInstance().log(PayNix::Core::LogLevel::SECURITY_ALERT, comp, msg)
}