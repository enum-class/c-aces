#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#ifdef __cplusplus
extern "C" {
#endif

typename uint64_t Coeff;

typename struct {
  Coeff *coeffs;
  size_t size;
} Polynomial;

typename struct {
  Polynomial *secret;
  size_t size;
} PolyArray;

uint64_t coef_sum(const Polynomial *);

uint64_t poly_degree(const Polynomial *);

int poly_fit(const Polynomial *poly, uint64_t mod);

int poly_mul(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result);

int poly_add(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result);

int poly_lshift(const Polynomial *poly1, const Polynomial *poly2,
                Polynomial *result);

int poly_mod(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result);

#ifdef __cplusplus
}
#endif

#endif
