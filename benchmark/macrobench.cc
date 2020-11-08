#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int cmd(std::string line, bool use_exception = true) {
  line += " > .benchbuild/std.log";
  line += " 2> .benchbuild/err.log";
  int status = system(line.c_str());
  if (status != 0 && use_exception) {
    throw std::runtime_error("command: \"" + line + "\" returned code " +
                             std::to_string(status));
  }
  return status;
}

std::optional<std::int64_t> try_build(std::string target) {
  auto t1 = std::chrono::high_resolution_clock::now();
  auto status =
      cmd("cmake --build .benchbuild --parallel 8 --target " + target, false);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  if (status == 0) {
    std::cout << target << " compiled in " << duration / 1000 / 1000 << "ms" << std::endl;
    return duration;
  } else {
    std::cout << target << " compilation failed" << std::endl;
    return std::nullopt;
  }
}

void bench(const size_t table_limit) {
  std::cout << "CONFIGURATION with table_limit = " << table_limit << std::endl;
  system("rm -rf .benchbuild");
  system("mkdir .benchbuild");

  cmd("cmake -S . -B .benchbuild "
      "-DCMAKE_TOOLCHAIN_FILE=toolchain/clang_debug.cmake -DTABLE_LIMIT=" +
      std::to_string(table_limit));

  cmd("cmake --build .benchbuild --parallel 8 --target macrobench");

  try_build("00_runtime");
  try_build("01_compile_time");

  cmd("rm -rf .benchbuild");
  std::cout << std::endl;
}

int main() {
  size_t table_limit = 10;

  while (table_limit < 100'000'000) {
    bench(table_limit);
    table_limit *= 10;
  }
  return 0;
}