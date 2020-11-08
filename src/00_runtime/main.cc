#include <cassert>
#include <iostream>
#include <vector>

auto get_primes_impl(size_t limit) {
  std::vector<char> not_prime(limit);
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

std::vector<size_t> get_primes(size_t limit) {
  std::vector<size_t> primes;
  auto not_prime = get_primes_impl(limit);

  auto it = primes.begin();
  for (size_t i = 0; i < not_prime.size(); ++i) {
    if (!not_prime[i]) {
      primes.emplace_back(i);
    }
  }
  return primes;
}

int main() {
  std::vector<size_t> primes = get_primes(TABLE_LIMIT);
  assert(primes[0] == 2);
  assert(primes[1] == 3);
  assert(primes[2] == 5);
  assert(primes[3] == 7);

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
