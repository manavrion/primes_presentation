#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

constexpr int TRY_COUNT = 101;

int dummy = 0;

namespace {
auto get_primes_impl(int limit) {
  std::vector<char> not_prime(limit);
  not_prime[0] = not_prime[1] = true;

  for (int i = 2; i < not_prime.size(); ++i) {
    if (!not_prime[i]) {
      if (i * i < not_prime.size()) {
        for (int j = i * i; j < not_prime.size(); j += i) {
          not_prime[j] = true;
        }
      }
    }
  }
  return not_prime;
}

std::vector<int> get_primes(int limit) {
  std::vector<int> primes;
  auto not_prime = get_primes_impl(limit);

  auto it = primes.begin();
  for (int i = 0; i < not_prime.size(); ++i) {
    if (!not_prime[i]) {
      primes.emplace_back(i);
    }
  }
  return primes;
}
}  // namespace

float sum(const std::vector<std::int64_t>& data) {
  assert(!data.empty());
  return std::accumulate(data.begin(), data.end(), float{});
}

float avg(const std::vector<std::int64_t>& data) {
  assert(!data.empty());
  return sum(data) / float(data.size());
}

float mid(std::vector<std::int64_t> data) {
  assert(!data.empty());
  std::sort(data.begin(), data.end());
  return data.at(data.size() / 2);
}

float _std(const std::vector<std::int64_t>& data) {
  assert(!data.empty());
  float n = data.size();
  float mean = sum(data) / n;

  float standardDeviation = 0.0;
  for (auto a : data) {
    standardDeviation += std::pow(a - mean, 2.0);
  }
  return std::sqrt(standardDeviation / n);
}

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

std::optional<std::int64_t> try_classic_impl(const size_t table_limit) {
  auto t1 = std::chrono::high_resolution_clock::now();
  std::vector<int> primes = get_primes(table_limit);
  auto status = !primes.empty();
  auto t2 = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

  if (status) {
    return duration;
  }
  return std::nullopt;
}

void try_classic(const size_t table_limit) {
  std::vector<std::int64_t> data;
  int try_count = TRY_COUNT;
  while (try_count--) {
    if (auto time = try_classic_impl(table_limit); time) {
      data.emplace_back(*time);
    }
  }

  if (!data.empty()) {
    std::cout << "avg = " << avg(data) / 1000 / 1000
              << "ms, mid = " << mid(data) / 1000 / 1000
              << "ms, std = " << _std(data) / 1000 / 1000 << "ms\t";
  } else {
    std::cout << "compilation failed\t";
  }

  std::cout << "runtime"
            << "\n";
  std::cout.flush();
}

std::optional<std::int64_t> try_build_impl(const size_t table_limit,
                                           std::string target, bool is_debug,
                                           std::string compiler) {
  std::string compile_command = compiler;
  compile_command += " -std=c++20";
  if (compiler == "clang++-12") {
    compile_command += " -stdlib=libc++";
    // compile_command += " -fconstexpr-depth=1000";
    // compile_command += " -fconstexpr-steps=1000000000";
  } else {
    // compile_command += " -fconstexpr-depth=1000";
    // compile_command += " -fconstexpr-loop-limit=1000000000";
    // compile_command += " -fconstexpr-ops-limit=1000000000";
  }
  compile_command += " ../src/" + target + "/main.cc";
  compile_command += " -o " + target;
  compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  compile_command += " -DRAND_DUMMY=" + std::to_string(++dummy);
  if (!is_debug) {
    compile_command += " -O3";
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  auto status = cmd(compile_command, false);
  auto t2 = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

  if (status == 0) {
    return duration;
  }
  return std::nullopt;
}

void try_build(const size_t table_limit, std::string target, bool is_debug,
               std::string compiler) {
  std::vector<std::int64_t> data;
  int try_count = TRY_COUNT;
  while (try_count--) {
    if (auto time = try_build_impl(table_limit, target, is_debug, compiler);
        time) {
      data.emplace_back(*time);
    }
  }

  std::cout << (is_debug ? "debug" : "release") << "\t";
  std::cout << compiler << "\t";

  if (!data.empty()) {
    std::cout << "avg = " << avg(data) / 1000 / 1000
              << "ms, mid = " << mid(data) / 1000 / 1000
              << "ms, std = " << _std(data) / 1000 / 1000 << "ms\t";
  } else {
    std::cout << "compilation failed\t";
  }

  std::cout << target << "\n";
  std::cout.flush();
}

std::optional<std::int64_t> try_build_03_build_step_impl(
    const size_t table_limit, bool is_debug, std::string compiler) {
  const std::string target = "03_build_step";
  std::string compile_command = compiler;
  if (compiler == "clang++-12") {
    compile_command += " -stdlib=libc++";
  }
  compile_command += " -std=c++20";
  compile_command += " ../src/" + target + "/main.cc";
  compile_command += " -o " + target;
  compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  compile_command += " -DRAND_DUMMY=" + std::to_string(++dummy);
  compile_command += " -O3";

  std::string generation_command = "echo " + std::to_string(table_limit) +
                                   " | ./03_build_step > "
                                   "03_build_step_gen.cc";

  std::string gen_compile_command = compiler;
  if (compiler == "clang++-12") {
    gen_compile_command += " -stdlib=libc++";
  }
  gen_compile_command += " -std=c++20";
  gen_compile_command += " 03_build_step_gen.cc";
  gen_compile_command += " -o 03_build_step_gen";
  gen_compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  gen_compile_command += " -DRAND_DUMMY=" + std::to_string(++dummy);
  if (!is_debug) {
    gen_compile_command += " -O3";
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  auto status1 = cmd(compile_command, false);
  auto status2 = cmd(generation_command, false, false);
  auto status3 = cmd(gen_compile_command, false);
  auto t2 = std::chrono::high_resolution_clock::now();

  auto status = status1 + status2 + status3;

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

  if (status == 0) {
    return duration;
  }
  return std::nullopt;
}

void try_build_03_build_step(const size_t table_limit, bool is_debug,
                             std::string compiler) {
  std::vector<std::int64_t> data;
  int try_count = TRY_COUNT;
  while (try_count--) {
    if (auto time =
            try_build_03_build_step_impl(table_limit, is_debug, compiler);
        time) {
      data.emplace_back(*time);
    }
  }

  std::cout << (is_debug ? "debug" : "release") << "\t";
  std::cout << compiler << "\t";

  if (!data.empty()) {
    std::cout << "avg = " << avg(data) / 1000 / 1000
              << "ms, mid = " << mid(data) / 1000 / 1000
              << "ms, std = " << _std(data) / 1000 / 1000 << "ms\t";
  } else {
    std::cout << "compilation failed\t";
  }

  std::cout << "03_build_step_gen"
            << "\n";
  std::cout.flush();
}

std::optional<std::int64_t>
try_build_03_build_step_gen_without_preprare_step_impl(const size_t table_limit,
                                                       bool is_debug,
                                                       std::string compiler) {
  const std::string target = "03_build_step";
  std::string compile_command = compiler;
  if (compiler == "clang++-12") {
    compile_command += " -stdlib=libc++";
  }
  compile_command += " -std=c++20";
  compile_command += " ../src/" + target + "/main.cc";
  compile_command += " -o " + target;
  compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  compile_command += " -DRAND_DUMMY=" + std::to_string(++dummy);
  compile_command += " -O3";

  std::string generation_command = "echo " + std::to_string(table_limit) +
                                   " | ./03_build_step > "
                                   "03_build_step_gen.cc";

  std::string gen_compile_command = compiler;
  if (compiler == "clang++-12") {
    gen_compile_command += " -stdlib=libc++";
  }
  gen_compile_command += " -std=c++20";
  gen_compile_command += " 03_build_step_gen.cc";
  gen_compile_command += " -o 03_build_step_gen_without_preprare_step";
  gen_compile_command += " -DTABLE_LIMIT=" + std::to_string(table_limit);
  gen_compile_command += " -DRAND_DUMMY=" + std::to_string(++dummy);
  if (!is_debug) {
    gen_compile_command += " -O3";
  }

  cmd(compile_command, false);

  auto t1 = std::chrono::high_resolution_clock::now();
  auto status2 = cmd(generation_command, false, false);
  auto status3 = cmd(gen_compile_command, false);
  auto t2 = std::chrono::high_resolution_clock::now();

  auto status = status2 + status3;

  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

  if (status == 0) {
    return duration;
  }
  return std::nullopt;
}

void try_build_03_build_step_gen_without_preprare_step(const size_t table_limit,
                                                       bool is_debug,
                                                       std::string compiler) {
  std::vector<std::int64_t> data;
  int try_count = TRY_COUNT;
  while (try_count--) {
    if (auto time = try_build_03_build_step_gen_without_preprare_step_impl(
            table_limit, is_debug, compiler);
        time) {
      data.emplace_back(*time);
    }
  }

  std::cout << (is_debug ? "debug" : "release") << "\t";
  std::cout << compiler << "\t";

  if (!data.empty()) {
    std::cout << "avg = " << avg(data) / 1000 / 1000
              << "ms, mid = " << mid(data) / 1000 / 1000
              << "ms, std = " << _std(data) / 1000 / 1000 << "ms\t";
  } else {
    std::cout << "compilation failed\t";
  }

  std::cout << "03_build_step_gen_without_preprare_step"
            << "\n";
  std::cout.flush();
}

void bench(const size_t table_limit) {
  std::cout << "CONFIGURATION with table_limit = " << table_limit << std::endl;

#if 0
  try_build(table_limit, "00_runtime", false, "clang++-12");
  try_build(table_limit, "01_compile_time", false, "clang++-12");
  try_build(table_limit, "02_compile_time_bits", false, "clang++-12");
  try_build_03_build_step_gen_without_preprare_step(table_limit, false,
                                                    "clang++-12");
  try_build_03_build_step(table_limit, false, "clang++-12");

  try_build(table_limit, "00_runtime", false, "g++-10");
  try_build(table_limit, "01_compile_time", false, "g++-10");
  try_build(table_limit, "02_compile_time_bits", false, "g++-10");
  try_build_03_build_step_gen_without_preprare_step(table_limit, false,
                                                    "g++-10");
  try_build_03_build_step(table_limit, false, "g++-10");
#endif

  try_classic(table_limit);

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
