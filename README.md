# PayNix
##### A lightweight and modern C++ billing software designed for fast, simple, and reliable invoice management.

---

## 🚀 About PayNix

**PayNix** is a lightweight and intuitive billing software built in C++ — designed for small businesses, shops, and freelancers who need a fast and clean billing solution. Whether you’re generating invoices, managing item lists, or tracking transactions, PayNix has you covered.

---

## ⭐ Key Features

- 🧾 Easy billing & invoice generation
- 📦 Item & inventory management
- 👤 Customer & employee management
- 🧮 Phone validation & data checks
- 📊 Clean console interface

---

## 🛠 Installation

Clone the repo and build:

```bash
git clone https://github.com/roshhellwett/PayNix.git
cd PayNix
mkdir build && cd build
cmake ..
make
./PayNix
```

```
name: Build PayNix

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Install dependencies
        run: sudo apt-get update && sudo apt-get install -y cmake g++

      - name: Configure
        run: |
          mkdir -p build
          cd build
          cmake ..

      - name: Build
        run: |
          cd build
          make

```
<p align="center">
  👨‍💻 Developed with ❤️ by <a href="https://github.com/roshhellwett">@roshhellwett</a><br>
  📬 Have suggestions? Open an issue or start a discussion<br><br>
  ⭐ <b>If you like this project, give it a star!</b>
</p>



