#include "common-test.h"
#include <Common.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int gcd_test() {
  assert(1ULL == gcd(1, 1));
  assert(1ULL == gcd(2, 1));
  assert(1ULL == gcd(1, 2));
  assert(2ULL == gcd(2, 2));
  assert(3ULL == gcd(6, 15));
  assert(3ULL == gcd(15, 6));
  assert(1ULL == gcd(7, 15));
  assert(1ULL == gcd(15, 7));
  assert(1ULL == gcd(7, 15));
  assert(3ULL == gcd(11112, 44445));
  return 1;
}

int xgcd_test() {
  Xgcd result;

  result = xgcd(7, 7);
  assert(result.gcd == 7);
  assert(result.a == 0);
  assert(result.b == 1);

  result = xgcd(2, 2);
  assert(result.gcd == 2);
  assert(result.a == 0);
  assert(result.b == 1);

  result = xgcd(1, 1);
  assert(result.gcd == 1);
  assert(result.a == 0);
  assert(result.b == 1);

  result = xgcd(1, 2);
  assert(result.gcd == 1);
  assert(result.a == 1);
  assert(result.b == 0);

  result = xgcd(5, 6);
  assert(result.gcd == 1);
  assert(result.a == -1);
  assert(result.b == 1);

  result = xgcd(19, 13);
  assert(result.gcd == 1);
  assert(result.a == -2);
  assert(result.b == 3);

  return 1;
}

int matrix2d_multiply_test() {
  const size_t dim = 3;
  size_t required_mem = dim * dim;
  uint64_t mem[3 * required_mem];

  Matrix2D m1;
  Matrix2D m2;
  Matrix2D result;

  m1.dim = dim;
  m2.dim = dim;
  result.dim = dim;
  m1.data = mem;
  m2.data = mem + required_mem;
  result.data = mem + 2 * required_mem;

  for (size_t i = 0; i < dim; ++i)
    for (size_t j = 0; j < dim; ++j)
      matrix2d_set(&m1, i, j, randrange(0, 96));
  matrix2d_eye(&m2);

  matrix2d_multiply(&m1, &m2, &result, 97);
  assert(0 == memcmp(m1.data, result.data, dim * dim * sizeof(uint64_t)));
  return 1;
}

int fill_random_invertible_pairs_test() {
  const size_t dim = 3;
  size_t required_mem = dim * dim;
  uint64_t mem[4 * required_mem];

  Matrix2D m1;
  Matrix2D m2;
  Matrix2D result;
  Matrix2D exp_result;

  m1.dim = dim;
  m2.dim = dim;
  result.dim = dim;
  exp_result.dim = dim;
  m1.data = mem;
  m2.data = mem + required_mem;
  result.data = mem + 2 * required_mem;
  exp_result.data = mem + 3 * required_mem;

  fill_random_invertible_pairs(&m1, &m2, 97, 600);

  matrix2d_multiply(&m1, &m2, &result, 97);
  matrix2d_eye(&exp_result);

  assert(0 ==
         memcmp(result.data, exp_result.data, dim * dim * sizeof(uint64_t)));
  return 1;
}

int run_common_tests() {
  assert(gcd_test());
  assert(xgcd_test());
  assert(matrix2d_multiply_test());
  assert(fill_random_invertible_pairs_test());
  return 1;
}
