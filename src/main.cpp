#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <unistd.h>
inline std::string red(const std::string &msg) {
  return "\033[1;31m" + msg + "\033[0m";
}
namespace fs = std::filesystem;

void add_watch_recursive(int fd, const fs::path &root, uint32_t mask) {
  for (const auto &entry : fs::recursive_directory_iterator(root)) {
    if (entry.is_directory()) {
      inotify_add_watch(fd, entry.path().c_str(), mask);
    }
  }

  inotify_add_watch(fd, root.c_str(), mask);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << red("[WarmLoader] Incorrect Usage!\n");
    std::cout << "      Usage: warmload <file|dir> --build <cmd> "
                 "--run <cmd>\n";
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
    std::cerr << red("[WarmLoader] Path does not exist!\n");
    return 1;
  }

  int fd = inotify_init1(IN_NONBLOCK);
  uint32_t mask = IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO;
  if (fs::is_directory(target)) {
    add_watch_recursive(fd, target, mask);
  } else {
    int wd = inotify_add_watch(fd, target.parent_path().c_str(), IN_MODIFY);
  }

  std::cout << red("[WarmLoader] ") << "Watching: " << target << '\n';

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
      std::cout << red("[WarmLoader] ") << "Change detected!\n";
      std::cout << red("[WarmLoader] ") << "Building...\n";
      if (running_pid > 0) {
        kill(running_pid, SIGTERM);
        int status;
        waitpid(running_pid, &status, 0);
      }
      int code = system(build_cmd.c_str());
      if (code != 0) {
        std::cerr << red("[WarmLoader] Build failed boohoo :(\n");
        return 1;
      }
      pid_t pid = fork();
      if (pid == 0) {
        execl("/bin/sh", "sh", "-c", run_cmd.c_str(), nullptr);
        perror("execl failed");
        _exit(1);
      } else if (pid > 0) {
        running_pid = pid;
        std::cout << red("[WarmLoader] ") << "Running...\n";
      } else {
        std::cerr << red("[WarmLoader] Running failed boohoo :(\n");
        return 1;
      }
    }
    usleep(100000); // 100ms
  }
  return 0;
}
