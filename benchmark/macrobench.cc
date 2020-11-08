#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int cmd(std::string line, bool use_exception = true, bool hide_out = true) {
  if (hide_out) {
    line += " > .benchbuild/std.log";
  }
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

  std::cout << "binary size: ";
  std::cout.flush();
  system(("du -h .benchbuild/src/" + target + "/" + target).c_str());

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  if (status == 0) {
    std::cout << target << " compiled in " << duration / 1000 / 1000 << "ms"
              << std::endl;
    return duration;
  }
  std::cout << target << " compilation failed" << std::endl;
  return std::nullopt;
}

std::optional<std::int64_t> try_build_03_build_step(const size_t table_limit) {
  auto t1 = std::chrono::high_resolution_clock::now();
  auto status1 = cmd(
      "cmake --build .benchbuild --parallel 8 --target 03_build_step", false);
  auto status2 = cmd("echo " + std::to_string(table_limit) +
                         " | .benchbuild/src/03_build_step/03_build_step > "
                         ".benchbuild/test.cc",
                     false, false);
  auto status3 =
      cmd("clang++-12 -stdlib=libc++ .benchbuild/test.cc -o .benchbuild/main "
          "-std=c++20 -fconstexpr-depth=1000 -fconstexpr-steps=1000000000",
          false);

  auto status = status1 + status2 + status3;

  auto t2 = std::chrono::high_resolution_clock::now();

  std::cout << "binary size: ";
  std::cout.flush();
  system("du -h .benchbuild/main");

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  if (status == 0) {
    std::cout << "03_build_step compiled in " << duration / 1000 / 1000 << "ms"
              << std::endl;
    return duration;
  }
  std::cout << "03_build_step compilation failed" << std::endl;
  return std::nullopt;
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
  try_build_03_build_step(table_limit);
  try_build("01_compile_time");
  try_build("02_compile_time_improved");

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