#include "Polynomial.h"

uint64_t coef_sum(const Polynomial *poly) {
  if (!poly || poly->size == 0)
    return 0;

  uint64_t sum = 0;
  for (int i = 0; i < poly->size; ++i)
    sum += poly->coeffs[i];

  return sum;
}

uint64_t poly_degree(const Polynomial *poly) {
  if (!poly || poly->size == 0)
    return 0;

  const uint64_t *value = poly->coeffs;
  size_t count = poly->size;
  value += count - 1;

  for (; count && !*value; count--)
    --value;

  return count;
}

int poly_fit(const Polynomial *poly, uint64_t mod) {
  if (!poly || poly->size == 0)
    return -1;

  for (int i = 0; i < poly->size; ++i)
    poly->coeffs[i] = poly->coeffs[i] % mod;

  return 0;
}

int poly_mul(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result) {
  // TODO: check

  for (int i = 0; i < poly->size; ++i)
    result->coeffs[i] = poly1->coeffs[i] * poly2->coeffs[i];
  return 0;
}

int poly_add(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result) {
  for (int i = 0; i < poly->size; ++i)
    result->coeffs[i] = poly1->coeffs[i] + poly2->coeffs[i];
  return 0;
}

int poly_lshift(const Polynomial *poly1, const Polynomial *poly2,
                Polynomial *result) {
  if (poly2->coeffs[0] != 1)
    return -1;

  uint64_t degree1 = poly_degree(poly1);
  uint64_t degree2 = poly_degree(poly2);

  if (degree1 < degree2)
    return -1;

  uint64_t a_d = poly1->coeffs[0];

  for (int i = 0; i < poly1->size; ++i) {
    if (i < poly2->size)
      result[i] = poly1->coeffs[i] - poly2->coeffs[i] * a_d;
    else
      result[i] = poly1->coeffs[i];
  }

  return 0;
}

int poly_mod(const Polynomial *poly1, const Polynomial *poly2) {
  while (poly_lshift(poly1, poly2, poly1) == 0) {
  }
}
