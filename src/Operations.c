#include "Operations.h"
#include "Polynomial.h"

int aces_add(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  for (int i = 0; i < dim; ++i) {
    poly_add(a->c1[i], b->c1[i], result->c1[i]);
    poly_mod(result->c1[i], u, result->c1[i]);
  }

  poly_add(a->c2, b->c2, result->c2);
  poly_mod(result->c2, u, result->c2);
  result->level = a->level + b->level;
}

int aces_mul(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  PolyArray t; // TODO:
  for (int k = 0; k < info->param.dim; ++k) {
    Polynomial tmp1;
    for (int i = 0; i < info->param.dim; ++i) {
      for (int j = 0; j < info->param.dim; ++j) {
        Polynomial tmp2;
        poly_mul(a.c1[i], b.c1[j], tmp2);
        poly_add_scaler(tmp2, info->pk.lambda[i][j][k], tmp2)
            poly_add(tmp1, tmp2, tmp1);
      }
    }
    t[k] = tmp1;
  }

  uint64_t mem1[info->param.dim * 2];
  Polynomial tmp1;
  Polynomial tmp2;
  set_polynomial(&tmp1, mem1, info->param.dim);
  set_polynomial(&tmp2, mem1 + info->param.dim, info->param.dim);

  for (int k = 0; k < info->param.dim; ++k) {
    poly_mul(a.c1[k], b.c2, tmp1);
    poly_mul(b.c1[k], a.c2, tmp2);
    poly_add(tmp1, tmp2, result.c1[k]);
    poly_sub_scaler(result.c1[k], t[k], result.c1[k]);
    poly_mod(result.c1[k], u);
  }

  // result.c2 = (a.c2 * b.c2) % u; // TODO:

  return 0;
}

int aces_refresh(CipherMessage *a, uint64_t k) {
  poly_sub_scaler(a->c2, k);
  a->level -= k;
  return 0;
}
