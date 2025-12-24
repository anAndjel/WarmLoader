# WarmLoader – Instant Build & Run on File Changes 🚀

**WarmLoader** is a blazing-fast, lightweight CLI tool that watches your source code and **automatically builds and runs your project** whenever you make changes. Perfect for C++ developers (or any language with build scripts) who hate the repetitive `make && ./app` cycle.

Spend less time building and more time creating. 💻✨

---

## Features ✨

* 🟢 **Automatic build & run** on file changes
* 🟡 **Debounced detection** – avoids multiple rebuilds on rapid edits
* 🔹 Works with **custom build and run commands** (`--build`, `--run`)
* 🔹 **Directory or file watching** (supports recursive watching)
* ⚡ **Lightweight C++ CLI** – minimal dependencies, maximum speed
* 🐈 Cat-proof… well, almost 😼 (watch those cables!)

---

## Installation ⚡

### 1️⃣ Clone the repository

`git clone [https://github.com/anAndjel/WarmLoader.git](https://github.com/anAndjel/WarmLoader.git)`

### 2️⃣ Build WarmLoader

```bash
cd WarmLoader
mkdir build && cd build
cmake ..
make
```

### 3️⃣ Move the binary into your PATH

`sudo cp warmloader /usr/local/bin/`

Now you can run `warmloader` from anywhere in your system.

---

## Usage 📦

### Basic command

From your project's root directory:

`warmloader <file-or-directory> --build <build-script> --run <run-script>`

**Example:**

`warmloader src/ --build ./build.sh --run ./run.sh`

**Parameters:**

* `<file-or-directory>` – file or folder to watch for changes
* `--build <script>` – your build script or command
* `--run <script>` – your run script or command

WarmLoader will automatically detect changes, rebuild your project, and restart it — no manual relaunching required.

---

### Help

`warmloader --help`

Displays usage instructions and available options.

---

### Tips & Notes 💡

* ⚠️ **Avoid watching your build directory** to prevent infinite rebuild loops.
* Works with **any build system** (Make, CMake, custom scripts).
* Debounce is **~700ms** to prevent multiple rebuilds from rapid saves.
* Always ensure your `--run` command points to a valid executable or script.

---

## Why WarmLoader? 🚀

* Stop wasting time on repetitive build/run cycles
* Focus on **writing code and testing ideas instantly**
* Lightweight and fast — perfect for **hobby projects, student work, or professional dev environments**

---

### Features at a glance

| Feature           | Description                                       |
| ----------------- | ------------------------------------------------- |
| Automatic rebuild | Detects source changes and rebuilds automatically |
| Run after build   | Kills previous run and starts a new executable    |
| Recursive watch   | Watches directories recursively                   |
| Custom commands   | Works with any build/run script or command        |
| Lightweight       | Minimal dependencies, fast performance            |

---

WarmLoader — **your project, always running, always up-to-date.** 💻✨
