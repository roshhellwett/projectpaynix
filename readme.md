# PROJECT PAYNIX

PayNix is a highly robust, terminal-based Point of Sale (POS) and inventory management application. Engineered with a "security-first" mindset, PayNix features encrypted data storage, strict buffer-overflow protections, atomic financial transactions, and role-based access control.

---

## ✨ Key Features

### 🔐 Security & Data Integrity
* **Role-Based Access Control (RBAC):** Distinct administrative and employee interfaces.
* **Brute-Force Lockout Gates:** Progressive time delays on failed login attempts.
* **Safe Memory Management:** Custom `SafeString` implementation prevents buffer overflows and mitigates scanner-paste injection attacks.
* **Binary File Protection:** All database files feature Magic Number validation (`PNIX`), strict schema versioning, and cryptographic checksums to prevent data tampering.
* **Schema Desync Gate:** Graceful schema migration and automatic legacy data archiving.
* **Graceful OS Intercepts:** Traps `SIGINT` and `SIGTERM` to safely flush buffers and lock vaults during unexpected shutdowns.

### 💰 Financial & Billing Engine
* **Integer Cent Math:** Eliminates floating-point inaccuracies for absolute financial safety.
* **Atomic Transactions:** "Reality Gate" validates inventory stock *before* processing to prevent race conditions or negative stock.
* **Live "Blind Biller" Terminal:** Real-time cart editing, stock viewing, and interactive quantity management.
* **JSON Export:** Automatically exports all finalized transactions to a secure JSON format.
* **Z-Reports:** Automated end-of-day gross revenue and audit report generation.

### 📦 Inventory Management
* Full CRUD operations for items (Add, Update, Soft-Delete).
* Stock validation and automated deductions.

---

## 🛠️ Prerequisites

To build and run PayNix, you will need:
* **CMake:** Version 3.20 or higher.
* **C++ Compiler:** C++20 compatible compiler (GCC, Clang, or MSVC).
* **Threads:** OS-level threading support (pthreads/Windows threads).

---

## 🚀 Build Instructions

PayNix uses CMake for cross-platform compilation. Follow these steps to build from source:

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/projectpaynix.git](https://github.com/yourusername/projectpaynix.git)
   cd projectpaynix
