#include "Aces.h"
#include "Aces-internal.h"
#include "Common.h"

#include <string.h>

int init_aces(uint64_t p, uint64_t q, uint64_t dim, Aces *aces) {
  aces->shared_info.param.dim = dim;
  aces->shared_info.param.N = 1;

  init_channel(&aces->shared_info.channel, p, q, 1);
  generate_u(&aces->shared_info.channel, &aces->shared_info.param,
             &aces->shared_info.pk.u);

  generate_secret(&aces->shared_info.channel, &aces->shared_info.param,
                  &aces->shared_info.pk.u, &aces->private_key.x,
                  &aces->shared_info.pk.lambda);
  generate_f0(&aces->shared_info.channel, &aces->shared_info.param,
              &aces->private_key.f0);
  generate_f1(&aces->shared_info.channel, &aces->shared_info.param,
              &aces->private_key.f0, &aces->private_key.x,
              &aces->shared_info.pk.u, &aces->private_key.f1);
  return 0;
}

int aces_encrypt(const Aces *aces, const uint64_t *message, size_t size,
                 CipherMessage *result) {
  if (size > 1)
    return -1;
  const size_t dim = aces->shared_info.param.dim;
  Coeff mem[3 * dim];
  Polynomial r_m;
  Polynomial e;
  Polynomial b;
  set_polynomial(&r_m, mem, dim);
  set_polynomial(&e, mem + dim, dim);
  set_polynomial(&b, mem + 2 * dim, dim);

  generate_error(aces->shared_info.channel.q, dim, *message, &r_m);
  generate_vanisher(aces->shared_info.channel.p, aces->shared_info.channel.q,
                    dim, &e);
  generate_linear(aces->shared_info.channel.p, aces->shared_info.channel.q, dim,
                  &b);

  Coeff tmp_mem[2 * dim];
  Polynomial tmp;

  // C1
  for (size_t i = 0; i < dim; ++i) {
    set_polynomial(&tmp, tmp_mem, 2 * dim);
    poly_mul(&b, &aces->private_key.f0.polies[i], &tmp,
             aces->shared_info.channel.q);
    poly_mod(&tmp, &aces->shared_info.pk.u, aces->shared_info.channel.q);
    memcpy(result->c1.polies[i].coeffs, tmp.coeffs, tmp.size * sizeof(Coeff));
  }

  // C2
  set_polynomial(&tmp, tmp_mem, 2 * dim);
  poly_add(&aces->private_key.f1, &e, &result->c2, aces->shared_info.channel.q);
  poly_mul(&result->c2, &b, &tmp, aces->shared_info.channel.q);
  poly_add(&tmp, &r_m, &tmp, aces->shared_info.channel.q);
  poly_mod(&tmp, &aces->shared_info.pk.u, aces->shared_info.channel.q);
  memcpy(result->c2.coeffs, tmp.coeffs, tmp.size * sizeof(Coeff));

  // Level
  result->level = aces->shared_info.channel.p;

  return 0;
}

int aces_decrypt(const Aces *aces, const CipherMessage *message, size_t size,
                 uint64_t *result) {
  if (size > 1)
    return -1;
  const size_t dim = aces->shared_info.param.dim;
  Coeff c0Tx_mem[2 * dim];
  Polynomial c0Tx;
  set_polynomial(&c0Tx, c0Tx_mem, 2 * dim);
  set_zero(&c0Tx);

  Coeff tmp_mem[2 * dim];
  Polynomial tmp;

  for (size_t i = 0; i < dim; ++i) {
    set_polynomial(&tmp, tmp_mem, 2 * dim);
    poly_mul(&message->c1.polies[i], &aces->private_key.x.polies[i], &tmp,
             aces->shared_info.channel.q);
    poly_add(&tmp, &c0Tx, &c0Tx, aces->shared_info.channel.q);
  }

  poly_fit(&c0Tx, aces->shared_info.channel.q);
  poly_sub(&message->c2, &c0Tx, &c0Tx, aces->shared_info.channel.q);

  *result = (coef_sum(&c0Tx) % aces->shared_info.channel.q) %
            aces->shared_info.channel.p;
  return 0;
}

int aces_add(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  for (size_t i = 0; i < info->param.dim; ++i) {
    poly_add(&a->c1.polies[i], &b->c1.polies[i], &result->c1.polies[i],
             info->channel.q);
    poly_mod(&result->c1.polies[i], &info->pk.u, info->channel.q);
  }

  poly_add(&a->c2, &b->c2, &result->c2, info->channel.q);
  poly_mod(&result->c2, &info->pk.u, info->channel.q);
  result->level = a->level + b->level;
  return 0;
}

int aces_mul(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  (void)a;
  (void)b;
  (void)info;
  (void)result;
  return 0;
}

int aces_refresh(const SharedInfo *info, CipherMessage *a, uint64_t k) {
  poly_sub_scaler(&a->c2, k * info->channel.p, &a->c2, info->channel.q);
  a->level -= k;
  return 0;
}
