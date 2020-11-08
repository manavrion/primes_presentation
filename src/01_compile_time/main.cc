#include <array>
#include <cassert>
#include <iostream>

namespace ranges {
template <typename C, typename T>
constexpr size_t count(C&& c, T&& t) {
  size_t res = 0;
  for (auto&& val : c) {
    if (val == t) {
      ++res;
    }
  }
  return res;
}
}  // namespace ranges

template <size_t Limit>
constexpr auto get_primes_impl() {
  std::array<bool, Limit> not_prime{};
  not_prime[0] = not_prime[1] = true;

  for (size_t i = 2; i < not_prime.size(); ++i) {
    if (!not_prime[i]) {
      if (i * i < not_prime.size()) {
        for (size_t j = i * i; j < not_prime.size(); j += i) {
          not_prime[j] = true;
        }
      }
    }
  }
  return not_prime;
}

template <size_t Limit>
constexpr auto get_primes() {
  constexpr auto not_prime = get_primes_impl<Limit>();

  std::array<size_t, ranges::count(not_prime, false)> primes{};
  auto it = primes.begin();
  for (size_t i = 0; i < not_prime.size(); ++i) {
    if (!not_prime[i]) {
      *it++ = i;
    }
  }
  return primes;
}

int main() {
  constexpr auto primes = get_primes<TABLE_LIMIT>();
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
