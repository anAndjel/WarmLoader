#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <signal.h>
#include <string>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>

inline std::string red(const std::string &msg) {
  return "\033[1;31m" + msg + "\033[0m";
}
namespace fs = std::filesystem;
using namespace std::chrono_literals;

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
  uint32_t mask = IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_MOVED_TO;
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

  std::unordered_set<std::string> modified_files;

  while (true) {
    int len;
    while ((len = read(fd, buf, sizeof(buf))) > 0) {
      int i = 0;
      while (i < len) {
        struct inotify_event *event = (struct inotify_event *)&buf[i];
        if (event->len > 0) {
          fs::path full_path = target / event->name;
          modified_files.insert(full_path.string());
        }
        i += sizeof(struct inotify_event) + event->len;
      }
      last = clock::now();
      dirty = true;
    }

    // debounce
    if (dirty && std::chrono::duration_cast<std::chrono::milliseconds>(
                     clock::now() - last)
                         .count() > 700) {
      dirty = false;

      if (!modified_files.empty()) {
        std::cout << red("[WarmLoader] ") << "Change detected!\n";
        std::cout << red("[WarmLoader] ") << "Building...\n";

        if (running_pid > 0) {
          kill(running_pid, SIGTERM);
          waitpid(running_pid, nullptr, 0);
        }

        int code = system(build_cmd.c_str());
        if (code != 0) {
          std::cerr << red("[WarmLoader] Build failed :(\n");
          modified_files.clear();
          continue;
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
          perror("fork failed");
        }

        modified_files.clear(); // reset for next batch
      }
    }

    usleep(100000); // 100ms
  }
  return 0;
}
