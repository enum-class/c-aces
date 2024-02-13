#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// gcd = ax + by
typedef struct {
  uint64_t gcd;
  int64_t a;
  int64_t b;
} Xgcd;

typedef struct {
  uint64_t first;
  uint64_t second;
} Pair;

uint64_t gcd(uint64_t x, uint64_t y);

Xgcd xgcd(uint64_t a, uint64_t b);

int are_coprime(uint64_t x, uint64_t y);

Pair randinverse(uint64_t value);

uint64_t randrange(uint64_t lower, uint64_t upper);
// factors(uint64_t value);

uint64_t normal_rand(double mean, double stddev);

static inline uint64_t max(uint64_t a, uint64_t b) { return a > b ? a : b; }

static inline uint64_t min(uint64_t a, uint64_t b) { return a < b ? a : b; }

static inline uint64_t clamp(uint64_t min_value, uint64_t max_value,
                             uint64_t value) {
  return max(min_value, min(max_value, value));
}

int fill_inv_random(size_t dim, uint64_t m[dim][dim], inv_m[dim][dim]);

int random_swap_transform(size_t dim, uint64_t m[dim][dim]) {}

int random_multiply_transform(size_t dim, uint64_t m[dim][dim]) {}

int random_linear_transform(size_t dim, uint64_t m[dim][dim]) {}

#ifdef __cplusplus
}
#endif

#endif
