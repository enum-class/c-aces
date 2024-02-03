#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

typename struct {
} Xgcd;

Xgcd xgcd(uint64_t a, uint64_t b);
randinverse(uint64_t value);
factors(uint64_t value);

uint64_t coprime_rand(uint64_t value);
uint64_t normal_rand(double mean, double stddev);
uint64_t randint(min, max);

#ifdef __cplusplus
}
#endif

#endif
