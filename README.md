<p align="center">
  <img src="https://github.com/roshhellwett/PayNix/blob/ec099998d78f5eb23f40a6002e7870ca9df5147a/img/logo_removebg.png" alt="PayNix Logo" width="200">
  <h1 align="center">PayNix</h1>
  <p align="center">
    💼 Modern Billing Software — Simple, Fast & Powerful
  </p>
<p align="center">
<img src="https://github.com/roshhellwett/PayNix/blob/e83f99682983aa0ed214cab9143707a0469a6176/img/img_one.png" width="250" height="300"/>
<img src="https://github.com/roshhellwett/PayNix/blob/e83f99682983aa0ed214cab9143707a0469a6176/img/img_two.png" width="250" height="300"/>
<img src="https://github.com/roshhellwett/PayNix/blob/e83f99682983aa0ed214cab9143707a0469a6176/img/img_three.png" width="250" height="300"/>
</p>
</p>

---

## 🚀 About PayNix

**PayNix** is a lightweight and intuitive billing software built in C++ — designed for small businesses, shops, and freelancers who need a fast and clean billing solution.  
Whether you’re generating invoices, managing item lists, or tracking transactions, PayNix has you covered.

---

## ⭐ Key Features

- 🧾 Easy billing & invoice generation
- 📦 Item & inventory management
- 👤 Customer & employee management
- 🧮 Phone validation & data checks
- 📊 Clean console interface

---

## 🖼 Screenshots

![Main Menu](https://github.com/roshhellwett/PayNix/blob/e83f99682983aa0ed214cab9143707a0469a6176/img/img_three.png)

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



