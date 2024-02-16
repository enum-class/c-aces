#include "Channel.h"

#include <math.h>

int init_channel(Channel *channel, uint64_t p, uint64_t q, uint64_t w) {
  channel->w = w;
  channel->p = p;
  channel->q = q;

  if (!(pow(p, 2) < q && are_coprime(p, q)))
    channel->q = (uint64_t)pow(p + 1, 2);

  return 0;
}

Matrix2D *lambda_get(Lambda *lambda, size_t idx) { return &lambda->data[idx]; }

int generate_u(const Channel *channel, const Parameters *param, Polynomial *u) {
  uint64_t nonzeros = randrange(
      param->dim / 2,
      param->dim - 1); // clamp(param->dim / 2, param->dim - 1, rand());
                       //  TODO: normal_rand(param->dim / 2, param->dim / 2));
  uint64_t zeroes = param->dim - nonzeros;
  u->coeffs[0] = 1;
  // nonzeros--;

  for (int i = 1; i < param->dim;) {
    if (nonzeros > 1) {
      u->coeffs[i++] = randrange(0, channel->q);
      nonzeros--;
    } else {
      break;
    }

    if (zeroes > 0) {
      uint64_t samp = clamp(0, zeroes, normal_rand(zeroes / 2, zeroes / 2));
      uint64_t zero = randrange(0, samp);
      for (int j = 0; j < zero; ++j)
        u->coeffs[i++] = 0;
      zeroes -= zero;
    }
  }

  u->coeffs[param->dim] = 0;
  u->coeffs[param->dim] = (int64_t)channel->q - (int64_t)coef_sum(u);
  return 0;
}

int generate_secret(const Channel *channel, const Parameters *param,
                    const Polynomial *u, PolyArray *secret, Lambda *lambda) {
  Matrix2D m;
  Matrix2D invm;

  m.dim = param->dim;
  invm.dim = param->dim;

  size_t required_mem = m.dim * m.dim;
  uint64_t mem[2 * required_mem];
  m.data = mem;
  invm.data = mem + required_mem;

  fill_random_invertible_pairs(&m, &invm, channel->q, 600);

  secret->size = m.dim;
  for (int i = 0; i < m.dim; ++i) {
    for (int j = 0; j < m.dim; ++j) {
      secret->polies[i].coeffs[j] = matrix2d_get(&m, j, i); // m[j][i];
    }
  }

  Matrix2D *arr = &m;
  Polynomial a_ij_poly; // TODO:

  Coeff miim[2 * secret->size];

  for (int i = 0; i < secret->size; ++i) {
    for (int j = 0; j < secret->size; ++j) {
      set_polynomial(&a_ij_poly, miim, 2 * secret->size);
      poly_mul(&secret->polies[i], &secret->polies[j], &a_ij_poly, channel->q);
      poly_mod(&a_ij_poly, u, channel->q);

      for (int row = 0; row < arr->dim; row++) {
        matrix2d_set(arr, row, j, a_ij_poly.coeffs[arr->dim - row - 1]);
      }
    }

    matrix2d_multiply(&invm, arr, lambda_get(lambda, i), channel->q);
  }

  return 0;
}

int generate_f0(const Channel *channel, const Parameters *param,
                PolyArray *f0) {
  for (int i = 0; i < f0->size; ++i) {
    for (int j = 0; j < f0->polies[i].size; j++)
      f0->polies[i].coeffs[j] = randrange(0, channel->q - 1);

    // TODO:
    // int shift_idx = randrange(0, f0[i].size - 1);
    // f0[i].polies->coeffs[shift_idx] =
    //    (f0[i].polies->coeffs[shift_idx] - sum(f0[i])) % channel->q;
  }

  return 0;
}

int generate_f1(const Channel *channel, const Parameters *param,
                const PolyArray *f0, const PolyArray *x, Polynomial *f1) {
  //  set_zero(f1);
  //  for (int i = 0; i < param->dim; ++i)
  //    poly_mul(f0[i], x[i], f1, channel->q);
  //
  //  poly_mod(f1, channel->u, f1, channel->q);
  return 0;
}
