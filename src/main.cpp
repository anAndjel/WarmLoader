#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "[WarmLoader] Incorrect Usage!\n";
    std::cout << "      Usage: warmload <file|dir>\n";
    return 1;
  }

  pid_t running_pid = -1;

  std::string build_cmd;
  std::string run_cmd;

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--build" && i + 1 < argc) {
      build_cmd = argv[++i];
    }
    if (std::string(argv[i]) == "--run" && i + 1 < argc) {
      run_cmd = argv[++i];
    }
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
      if (running_pid > 0) {
        kill(running_pid, SIGTERM);
        waitpid(running_pid, nullptr, 0);
      }
      int code = system(build_cmd.c_str());
      if (code != 0) {
        std::cout << "[WarmLoader] Build failed boohoo :(\n";
        return 1;
      }
      pid_t pid = fork();
      if (pid == 0) {
        execl("/bin/sh", "sh", "-c", run_cmd.c_str(), nullptr);
      }
    }
    usleep(100000); // 100ms
  }

  return 0;
}
