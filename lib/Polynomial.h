#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef uint64_t Coeff;

typedef struct {
  Coeff *coeffs;
  size_t size;
} Polynomial;

typedef struct {
  Polynomial *polies;
  size_t size;
} PolyArray;

int poly_equal(const Polynomial *, const Polynomial *);
void poly_free(Polynomial *poly);

int set_polynomial(Polynomial *p, Coeff *coeffs, size_t size);

int get_polynomial(Polynomial *p, size_t size);

int get_polyarray(PolyArray *p, size_t size);

int set_zero(const Polynomial *);

uint64_t coef_sum(const Polynomial *);

uint64_t poly_degree(const Polynomial *);

int poly_fit(Polynomial *poly, uint64_t mod);

int poly_mul(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result, uint64_t mod);

int poly_add(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result, uint64_t mod);

int poly_sub(const Polynomial *poly1, const Polynomial *poly2,
             Polynomial *result, uint64_t mod);

int poly_lshift(const Polynomial *poly1, const Polynomial *poly2,
                Polynomial *result, uint64_t mod);

int poly_mod(Polynomial *, const Polynomial *, uint64_t);

int poly_sub_scaler(const Polynomial *, uint64_t, Polynomial *, uint64_t);

int poly_add_scaler(const Polynomial *, uint64_t, Polynomial *, uint64_t);

#ifdef __cplusplus
}
#endif

#endif
