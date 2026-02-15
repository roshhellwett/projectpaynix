#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cctype>
#include "Models.hpp"

namespace PayNix::Security {
    constexpr uint64_t hashPassword(const std::string& password) {
        uint64_t hash = 0xcbf29ce484222325ull;
        for (char c : password) {
            hash ^= static_cast<uint64_t>(c);
            hash *= 0x100000001b3ull;
        }
        return hash;
    }

    template<typename T>
    uint64_t calculateDataChecksum(const std::vector<T>& data) {
        uint64_t hash = 0xcbf29ce484222325ull;
        const char* bytes = reinterpret_cast<const char*>(data.data());
        size_t size = data.size() * sizeof(T);
        for (size_t i = 0; i < size; ++i) {
            hash ^= static_cast<uint64_t>(bytes[i]);
            hash *= 0x100000001b3ull;
        }
        return hash;
    }

    inline bool isValidPhone(const std::string& phone) {
        if (phone.length() != 10 && phone != "0000000000") return false; // Allowed Walk-In Gate
        return std::all_of(phone.begin(), phone.end(), [](unsigned char c){ return std::isdigit(c); });
    }

    inline std::string sanitizeInput(std::string input) {
        if (input.length() > Core::MAX_INPUT_LENGTH) {
            input = input.substr(0, Core::MAX_INPUT_LENGTH); // Buffer Overflow/Scanner Paste Gate
        }
        input.erase(std::remove_if(input.begin(), input.end(), [](char c){
            return c < 32 || c > 126; 
        }), input.end());
        return input;
    }
}