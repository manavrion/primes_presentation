#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>

namespace {
template <int Limit>
constexpr auto get_primes() {
  constexpr int base = 64;
  constexpr int not_prime_size = Limit / base + bool(Limit % base);
  auto get = [](int i, auto& not_prime) {
    return bool(not_prime[i / base] & (uint64_t{1} << (i % base)));
  };
  auto set = [](int i, auto& not_prime) {
    not_prime[i / base] |= (uint64_t{1} << (i % base));
  };
  constexpr auto not_prime = [&]() {
    std::array<uint64_t, not_prime_size> not_prime{};
    set(0, not_prime);
    set(1, not_prime);

    for (int i = 2; i < Limit; ++i) {
      if (!get(i, not_prime)) {
        if (i * i < Limit) {
          for (int j = i * i; j < Limit; j += i) {
            set(j, not_prime);
          }
        }
      }
    }
    return not_prime;
  }();
  constexpr auto size = [&]() {
    int res = 0;
    for (int i = 0; i < Limit; ++i) {
      if (!get(i, not_prime)) {
        ++res;
      }
    }
    return res;
  }();

  std::array<int, size> primes{};
  auto it = primes.begin();
  for (int i = 0; i < Limit; ++i) {
    if (!get(i, not_prime)) {
      *it++ = i;
    }
  }
  return primes;
}
}  // namespace

constexpr auto primes = get_primes<TABLE_LIMIT>();

int main() {
  static_assert(primes[0] == 2);
  static_assert(primes[1] == 3);
  static_assert(primes[2] == 5);
  static_assert(primes[3] == 7);

  std::int64_t i = 0;
  while (std::cin >> i && i != -1) {
    if (0 <= i && i < primes.size()) {
      std::cout << primes[i] << '\n';
    } else {
      std::cout << "incorrect index\n";
    }
  }

  return 0;
}
