#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <algorithm>
#include "Core.hpp"

namespace PayNix::Models {

    template<size_t N>
    struct SafeString {
        std::array<char, N> data;
        SafeString() { data.fill(0); }
        SafeString(const std::string& str) {
            data.fill(0);
            std::copy_n(str.begin(), std::min(str.length(), N - 1), data.begin());
        }
        std::string toString() const { return std::string(data.data()); }
        bool operator==(const SafeString& other) const { return data == other.data; }
    };

    #pragma pack(push, 1) 
    struct FileHeader {
        uint32_t magic;
        uint32_t version;
        uint32_t recordCount;
        uint64_t dataChecksum;
    };

    struct Admin {
        SafeString<32> adminId;
        uint64_t passwordHash;
    };

    struct Employee {
        SafeString<20> empId;
        SafeString<64> name;
        SafeString<16> dob;
        uint64_t passwordHash;
    };

    struct Item {
        SafeString<32> id;
        SafeString<64> name;
        Core::Currency priceCents; 
        uint32_t stockQuantity;    
        bool isDeleted;
    };

    struct BillItem {
        SafeString<32> itemId;
        SafeString<64> itemName;
        Core::Currency priceCents;
        uint32_t quantity;
        Core::Currency totalCents;
    };

    struct BillHeader {
        SafeString<32> billId;
        SafeString<64> customerName;
        SafeString<16> phone;
        SafeString<32> datetime;
        uint32_t itemCount;
        Core::Currency grandTotalCents;
        std::array<BillItem, Core::MAX_BILL_ITEMS> items;
    };
    #pragma pack(pop)
}