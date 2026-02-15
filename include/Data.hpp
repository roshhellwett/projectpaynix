#pragma once
#include <vector>
#include <fstream>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include "Core.hpp"
#include "Models.hpp"
#include "Security.hpp"

namespace PayNix::Data {
    template<typename T>
    class SecureRepository {
        std::string filename;
        std::mutex repoMutex; 
    public:
        explicit SecureRepository(std::string file) : filename(std::move(file)) {}

        std::vector<T> loadAll() {
            std::lock_guard<std::mutex> lock(repoMutex);
            std::vector<T> records;
            if (!std::filesystem::exists(filename)) return records;

            std::ifstream in(filename, std::ios::binary);
            if (!in) {
                LOG_ERR("SafeIO", "Failed to open file: " + filename);
                return records;
            }

            Models::FileHeader header;
            if (!in.read(reinterpret_cast<char*>(&header), sizeof(header))) return records;

            if (header.magic != Core::FILE_MAGIC) {
                LOG_SEC("SafeIO", "Corrupt signature: " + filename);
                throw std::runtime_error("Magic Mismatch");
            }

            // Schema Desync Bomb Gate: Gracefully archive old versions
            if (header.version != Core::APP_VERSION) {
                LOG_SEC("SafeIO", "Schema version mismatch. Archiving old data to .bak: " + filename);
                in.close();
                std::filesystem::rename(filename, filename + ".bak");
                return records; // Return empty array, start fresh securely
            }

            records.resize(header.recordCount);
            if (header.recordCount > 0) {
                in.read(reinterpret_cast<char*>(records.data()), header.recordCount * sizeof(T));
            }

            if (Security::calculateDataChecksum(records) != header.dataChecksum) {
                LOG_SEC("SafeIO", "Data checksum failed for file: " + filename);
                throw std::runtime_error("Checksum Mismatch");
            }
            return records;
        }

        bool saveAll(const std::vector<T>& records) {
            std::lock_guard<std::mutex> lock(repoMutex);
            std::string tempFile = filename + ".tmp";
            std::ofstream out(tempFile, std::ios::binary);
            if (!out) return false;

            Models::FileHeader header{ Core::FILE_MAGIC, Core::APP_VERSION, static_cast<uint32_t>(records.size()), Security::calculateDataChecksum(records) };
            out.write(reinterpret_cast<const char*>(&header), sizeof(header));
            if (header.recordCount > 0) out.write(reinterpret_cast<const char*>(records.data()), records.size() * sizeof(T));
            out.flush();
            if(out.fail()) {
                out.close(); std::filesystem::remove(tempFile); return false;
            }
            out.close();

            try {
                if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
                std::filesystem::rename(tempFile, filename);
                return true;
            } catch (...) { return false; }
        }
    };
}