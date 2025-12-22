# WarmLoader – Instant Build & Run on File Changes 🚀

**WarmLoader** is a lightweight, fast CLI tool that watches your source code and **automatically builds and runs your project** whenever you make changes. Perfect for C++ developers who hate repetitive `make && ./app` cycles.  

---

## Features ✨

- 🟢 **Automatic build & run** on file changes  
- 🟡 **Debounced changes** – avoids multiple rebuilds on rapid edits  
- 🔹 Works with **custom build and run commands** (`--build`, `--run`)  
- 🔹 **Directory or file watching** (supports recursive watching)  
- 🔹 Blazzing fast lightweight C++ CLI ⚡⚡⚡⚡⚡

---

## Installation ⚡

### Step 1 - Clone the repository

```bash
git clone https://github.com/yourusername/WarmLoader.git
```

### Step 2 - Building

```bash
cd WarmLoader
mkdir build && cd build
cmake ..
make
```

### Step 3 – Move binary to PATH

```bash
sudo cp warmloader /usr/local/bin/
```

## Usage 📦

### Simply run it from your project's root directory
```bash
warmloader <file or directory> --build <your build command> --run <your run command>
```
