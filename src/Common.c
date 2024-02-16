#include "Common.h"

#include <stdlib.h>
#include <string.h>

uint64_t normal_rand(double mean, double stddev) {
  // TODO:
  (void)mean;
  (void)stddev;
  return 0;
}

uint64_t gcd(uint64_t x, uint64_t y) {
  while (x != y) {
    if (x > y)
      x -= y;
    else
      y -= x;
  }

  return x;
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

  Xgcd result = {.gcd = x, .a = prev_a, .b = prev_b};
  return result;
}

int are_coprime(uint64_t x, uint64_t y) { return !(gcd(x, y) > 1); }

uint64_t randrange(uint64_t lower, uint64_t upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

Pair randinverse(uint64_t value) {
  uint64_t a = randrange(2, value - 1);
  while (!are_coprime(a, value))
    a = randrange(2, value - 1);

  Xgcd result = xgcd(a, value);

  Pair p = {.first = a, .second = result.a > 0 ? result.a : result.a + value};
  return p;
}

int matrix2d_multiply(Matrix2D *m, Matrix2D *invm, Matrix2D *result,
                      uint64_t mod) {
  size_t dim = m->dim;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      matrix2d_set(result, i, j, 0);
      for (int k = 0; k < dim; k++) {
        matrix2d_set(result, i, j,
                     matrix2d_get(result, i, j) +
                         matrix2d_get(m, i, k) * matrix2d_get(invm, k, j));
      }
      matrix2d_set(result, i, j, matrix2d_get(result, i, j) % mod);
    }
  }

  return 0;
}

int swap_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod) {
  size_t dim = m->dim;
  uint64_t tmp;
  uint64_t tmp_row[dim];

  size_t source = randrange(1, dim - 1);
  size_t target = randrange(0, source - 1);

  for (size_t row = 0; row < dim; ++row) {
    tmp = matrix2d_get(m, row, source);
    matrix2d_set(m, row, source, matrix2d_get(m, row, target));
    matrix2d_set(m, row, target, tmp);
  }

  memcpy(tmp_row, matrix2d_row(invm, source), sizeof(uint64_t) * dim);
  memcpy(matrix2d_row(invm, source), matrix2d_row(invm, target),
         sizeof(uint64_t) * dim);
  memcpy(matrix2d_row(invm, target), tmp_row, sizeof(uint64_t) * dim);

  return 0;
}

int scale_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod) {
  size_t dim = m->dim;
  Pair scale = randinverse(mod);
  for (size_t idx = 0; idx < dim * dim; ++idx) {
    m->data[idx] = (m->data[idx] * scale.first) % mod;
    invm->data[idx] = (invm->data[idx] * scale.second) % mod;
  }
  return 1;
}

int linear_mix_transform(Matrix2D *m, Matrix2D *invm, uint64_t mod) {
  size_t dim = m->dim;
  size_t scale = randrange(1, mod - 1);
  size_t source = randrange(1, dim - 1);
  size_t target = randrange(0, source - 1);

  for (size_t row = 0; row < dim; ++row) {
    matrix2d_set(
        m, row, target,
        (matrix2d_get(m, row, target) + matrix2d_get(m, row, source) * scale) %
            mod);
    matrix2d_set(invm, source, row,
                 (matrix2d_get(invm, source, row) +
                  matrix2d_get(invm, target, row) * (mod - scale)) %
                     mod);
  }

  return 0;
}

int matrix2d_eye(Matrix2D *m) {
  size_t dim = m->dim;
  memset(m->data, 0, dim * dim * sizeof(uint64_t));

  for (size_t row = 0; row < dim; ++row)
    matrix2d_set(m, row, row, 1);

  return 0;
}

int fill_random_invertible_pairs(Matrix2D *m, Matrix2D *invm, uint64_t mod,
                                 size_t iterations) {
  const Transformer transformers[TRANSFORM_COUNT] = {
      &swap_transform, &scale_transform, &linear_mix_transform};

  matrix2d_eye(m);
  matrix2d_eye(invm);

  for (int i = 0; i < iterations; ++i) {
    size_t select = randrange(0, TRANSFORM_COUNT - 1);
    transformers[select](m, invm, mod);
  }
}
