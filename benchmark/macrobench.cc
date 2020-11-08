#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int cmd(std::string line, bool use_exception = true, bool hide_out = true) {
  if (hide_out) {
    line += " > std.log";
  }
  line += " 2> err.log";
  int status = system(line.c_str());
  if (status != 0 && use_exception) {
    throw std::runtime_error("command: \"" + line + "\" returned code " +
                             std::to_string(status));
  }
  return status;
}

std::optional<std::int64_t> try_build(const size_t table_limit,
                                      std::string target,
                                      bool is_debug = false) {
  std::string compile_command = "clang++-12";
  compile_command += " -stdlib=libc++";
  compile_command += " -std=c++20";
  compile_command += " -fconstexpr-depth=1000";
  compile_command += " -fconstexpr-steps=1000000000";
  compile_command += " ../src/" + target + "/main.cc";
  compile_command += " -o " + target;
  compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  if (!is_debug) {
    compile_command += " -O3";
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  auto status = cmd(compile_command, false);
  auto t2 = std::chrono::high_resolution_clock::now();

  std::string du_command = "du -h " + target;

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  if (status == 0) {
    std::cout << duration / 1000 / 1000 << "ms \t";
  } else {
    std::cout << "compilation failed \n";
  }

  std::cout.flush();
  system(du_command.c_str());

  if (status == 0) {
    return duration;
  }
  return std::nullopt;
}

std::optional<std::int64_t> try_build_03_build_step(const size_t table_limit,
                                                    bool is_debug = false) {
  const std::string target = "03_build_step";
  std::string compile_command = "clang++-12";
  compile_command += " -stdlib=libc++";
  compile_command += " -std=c++20";
  compile_command += " -fconstexpr-depth=1000";
  compile_command += " -fconstexpr-steps=1000000000";
  compile_command += " ../src/" + target + "/main.cc";
  compile_command += " -o " + target;
  compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  compile_command += " -O3";

  std::string generation_command = "echo " + std::to_string(table_limit) +
                                   " | ./03_build_step > "
                                   "03_build_step_gen.cc";

  std::string gen_compile_command = "clang++-12";
  gen_compile_command += " -stdlib=libc++";
  gen_compile_command += " -std=c++20";
  gen_compile_command += " -fconstexpr-depth=1000";
  gen_compile_command += " -fconstexpr-steps=1000000000";
  gen_compile_command += " 03_build_step_gen.cc";
  gen_compile_command += " -o 03_build_step_gen";
  gen_compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  if (!is_debug) {
    gen_compile_command += " -O3";
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  auto status1 = cmd(compile_command, false);
  auto status2 = cmd(generation_command, false, false);
  auto status3 = cmd(gen_compile_command, false);
  auto t2 = std::chrono::high_resolution_clock::now();

  auto status = status1 + status2 + status3;

  std::string du_command = "du -h 03_build_step_gen";

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
  if (status == 0) {
    std::cout << duration / 1000 / 1000 << "ms \t";
  } else {
    std::cout << "compilation failed \n";
  }

  std::cout.flush();
  system(du_command.c_str());

  if (status == 0) {
    return duration;
  }
  return std::nullopt;
}

void bench(const size_t table_limit) {
  std::cout << "CONFIGURATION with table_limit = " << table_limit << std::endl;

  try_build(table_limit, "00_runtime");
  try_build(table_limit, "01_compile_time");
  try_build(table_limit, "02_compile_time_improved");
  try_build_03_build_step(table_limit);

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
