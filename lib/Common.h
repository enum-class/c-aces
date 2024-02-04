#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

// gcd = ax + by
typename struct {
  uint64_t gcd;
  int64_t a;
  int64_t b;
} Xgcd;

Xgcd xgcd(uint64_t a, uint64_t b);
randinverse(uint64_t value);
factors(uint64_t value);

uint64_t normal_rand(double mean, double stddev);

uint64_t max(uint64_t a, uint64_t b) { a > b ? a : b; }

uint64_t min(uint64_t a, uint64_t b) { a < b ? a : b; }

uint64_t clamp(uint64_t min_value, uint64_t max_value, uint64_t value) {
  return max(min_value, min(max_value, value));
}

#ifdef __cplusplus
}
#endif

#endif
