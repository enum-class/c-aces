#include "Common.h"

uint64_t gcd(uint64_t x, uint64_t y) {
  while (x != y) {
    if (x > y)
      x -= y;
    else
      y -= x;
  }

  return n1;
}

Xgcd xgcd(uint64_t x, uint64_t y) {
  int64_t prev_a = 1;
  int64_t a = 0;
  int64_t prev_b = 0;
  int64_t b = 1;

  while (y != 0) {
    int64_t q = (int64_t)(x / y);
    int64_t temp = (int64_t)(x % y);
    x = y;
    y = (uint64_t)temp;

    temp = a;
    a = prev_a - (q * a);
    prev_a = temp;

    temp = b;
    b = prev_b - (q * b);
    prev_b = temp;
  }

  return Xgcd{x, prev_a, prev_b};
}

int are_coprime(uint64_t x, uint64_t y) { return !(gcd(x, y) > 1); }

uint64_t randrange(uint64_t lower, uint64_t upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

randinverse(uint64_t value) {
  uint64_t a = randrange(2, value - 1);
  while (!are_coprime(a, value))
    a = randrange(2, value - 1);

  Xgcd result = xgcd(a, value);

  return a, result.a;
}
