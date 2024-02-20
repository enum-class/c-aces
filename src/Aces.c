#include "Aces.h"
#include "Aces-internal.h"
#include "Common.h"
#include <stddef.h>
#include <stdint.h>

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
}

int aces_encrypt(const Aces *aces, const uint64_t *message, size_t size,
                 CipherMessage *result) {
  // Polynomial r_m; // TODO: size aces->info.param.dim
  // Polynomial e;
  // Polynomial b;

  // for (int i = 0; i < size; ++i) {
  //   generate_error(aces->info.channel.q, aces->info.param.dim, message[i],
  //                  &r_m);
  //   uint64_t k = generate_vanisher(aces->info.channel.p,
  //   aces->info.channel.q,
  //                                  aces->info.param.dim, &e);
  //   generate_linear(aces->info.channel.p, aces->info.channel.q,
  //                   aces->info.param.dim, &b);

  //  for (int j = 0; j < aces->info.param.dim; ++j) {
  //    poly_mul(&b, &(aces->privte_key.f0.polys[j]), &(result[i].c1.polys[j]),
  //             aces->info.channel.q);
  //    poly_mod(&(result[i].c1.polys[j]), &(aces->info.pk.u),
  //             &(result[i].c1.polys[j]), aces->info.channel.q);
  //  }

  //  // result[i].c2 = (r_m + b * (self.f1 + e)) % self.u; // TODO
  //  result[i].level = k * sum(b) % aces->info.channel.q;
  //}

  return 0;
}

int aces_decrypt(const Aces *aces, const CipherMessage *message, size_t size,
                 uint64_t *result) {
  // Polynomial c0Tx;
  // Polynomial tmp;
  // Polynomial m_pre;
  // set_zero(&c0Tx);

  // for (int i = 0; i < size; ++i) {
  //   for (int j = 0; j < aces->info.param.dim; ++j) {
  //     poly_mul(&(message[i].c1.polys[j]), &(aces->privte_key.x.polys[j]),
  //     &tmp,
  //              aces->info.channel.q);
  //     poly_add(&c0Tx, &tmp, &c0Tx, aces->info.channel.q);
  //   }

  //  poly_sub(&(message[i].c2), &c0Tx, &m_pre, aces->info.channel.q);
  //  result[i] = (sum(m_pre) % aces->info.channel.q) % aces->info.channel.p;
  //}

  return 0;
}

int aces_add(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  for (int i = 0; i < info->param.dim; ++i) {
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
  // PolyArray t; // TODO:
  // for (int k = 0; k < info->param.dim; ++k) {
  //   Polynomial tmp1;
  //   for (int i = 0; i < info->param.dim; ++i) {
  //     for (int j = 0; j < info->param.dim; ++j) {
  //       Polynomial tmp2;
  //       poly_mul(a.c1[i], b.c1[j], tmp2);
  //       poly_add_scaler(tmp2, info->pk.lambda[i][j][k], tmp2)
  //           poly_add(tmp1, tmp2, tmp1);
  //     }
  //   }
  //   t[k] = tmp1;
  // }

  // uint64_t mem1[info->param.dim * 2];
  // Polynomial tmp1;
  // Polynomial tmp2;
  // set_polynomial(&tmp1, mem1, info->param.dim);
  // set_polynomial(&tmp2, mem1 + info->param.dim, info->param.dim);

  // for (int k = 0; k < info->param.dim; ++k) {
  //   poly_mul(a.c1[k], b.c2, tmp1);
  //   poly_mul(b.c1[k], a.c2, tmp2);
  //   poly_add(tmp1, tmp2, result.c1[k]);
  //   poly_sub_scaler(result.c1[k], t[k], result.c1[k]);
  //   poly_mod(result.c1[k], u);
  // }

  // result.c2 = (a.c2 * b.c2) % u; // TODO:

  return 0;
}

int aces_refresh(const SharedInfo *info, CipherMessage *a, uint64_t k) {
  poly_sub_scaler(&a->c2, k * info->channel.p, &a->c2, info->channel.q);
  a->level -= k;
  return 0;
}
