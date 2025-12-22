#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "[WarmLoader] Incorrect Usage!\n";
    std::cout << "      Usage: warmload <file|dir>\n";
    return 1;
  }

  fs::path target = argv[1];

  if (!fs::exists(target)) {
    std::cerr << "[WarmLoader] Path does not exist!\n";
    return 1;
  }

  int fd = inotify_init1(IN_NONBLOCK);

  int wd = inotify_add_watch(fd, target.parent_path().c_str(), IN_MODIFY);

  std::cout << "[WarmLoader] Watching: " << target << '\n';

  char buf[4096];
  using clock = std::chrono::steady_clock;
  auto last = clock::now();
  bool dirty = false;

  while (true) {
    int len = read(fd, buf, sizeof(buf));
    if (len > 0) {
      dirty = true;
      last = clock::now();
    }

    if (dirty && std::chrono::duration_cast<std::chrono::milliseconds>(
                     clock::now() - last)
                         .count() > 300) {
      dirty = false;
      std::cout << "[WarmLoader] Stable change detected!\n";
      std::cout << "[WarmLoader] Building...\n";
      system("echo BUILD");
    }
    usleep(100000); // 100ms
  }

  return 0;
}
