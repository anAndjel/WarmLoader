# WarmLoader – Instant Build & Run on File Changes 🚀

**WarmLoader** is a lightweight, fast CLI tool that watches your source code and **automatically builds and runs your project** whenever you make changes. Perfect for C++ developers who hate repetitive `make && ./app` cycles.  

---

## Features ✨

- 🟢 **Automatic build & run** on file changes  
- 🟡 **Debounced changes** – avoids multiple rebuilds on rapid edits  
- 🔹 Works with **custom build and run commands** (`--build`, `--run`)  
- 🔹 **Directory or file watching** (supports recursive watching)  
- 🔹 Lightweight C++ CLI – no Node.js, Python, or extra deps  

---

## Installation ⚡

### Option 1 – Move binary to PATH (recommended)

```bash
git clone https://github.com/yourusername/WarmLoader.git
cd WarmLoader
mkdir build && cd build
cmake ..
make
sudo cp warmloader /usr/local/bin/

