#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define TRANSFORM_COUNT 3

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

typedef struct {
  size_t dim;
  uint64_t *data;
} Matrix2D;

static inline uint64_t *matrix2d_row(Matrix2D *matrix, size_t row) {
  return matrix->data + row * matrix->dim;
}

static inline uint64_t matrix2d_get(Matrix2D *matrix, size_t row, size_t col) {
  size_t dim = matrix->dim;
  return matrix->data[row * dim + col];
}

static inline void matrix2d_set(Matrix2D *matrix, size_t row, size_t col,
                                uint64_t value) {
  size_t dim = matrix->dim;
  matrix->data[row * dim + col] = value;
}

uint64_t gcd(uint64_t x, uint64_t y);

Xgcd xgcd(uint64_t a, uint64_t b);

int are_coprime(uint64_t x, uint64_t y);

Pair randinverse(uint64_t value);

uint64_t randrange(uint64_t lower, uint64_t upper);

uint64_t normal_rand(double mean, double stddev);

static inline uint64_t max(uint64_t a, uint64_t b) { return a > b ? a : b; }

static inline uint64_t min(uint64_t a, uint64_t b) { return a < b ? a : b; }

static inline uint64_t clamp(uint64_t min_value, uint64_t max_value,
                             uint64_t value) {
  return max(min_value, min(max_value, value));
}

int matrix2d_multiply(Matrix2D *m, Matrix2D *invm, Matrix2D *result,
                      uint64_t mod);

typedef int (*Transformer)(Matrix2D *m, Matrix2D *invm, uint64_t mod);

int swap_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod);
int linear_mix_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod);
int scale_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod);

int matrix2d_eye(Matrix2D *m);
int fill_random_invertible_pairs(Matrix2D *m, Matrix2D *invm, uint64_t mod,
                                 size_t iterations);

#ifdef __cplusplus
}
#endif

#endif
