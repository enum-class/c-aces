#include "Aces.h"
#include "Common.h"
#include <stddef.h>

static int generate_error(uint64_t q, uint64_t dim, uint64_t message,
                          Polynomial *rm) {
  for (int i = 0; i < rm->size; ++i)
    rm->coeffs[i] = randrange(0, q);

  // rm->coeffs[0] += m - sum(rm);
  rm->coeffs[0] %= q;
  return 0;
}

static uint64_t generate_vanisher(uint64_t p, uint64_t q, uint64_t dim,
                                  Polynomial *e) {
  uint64_t k = randrange(0, 1) < EPROB ? 0 : 1;
  for (int i = 0; i < e->size; ++i)
    e->coeffs[i] = randrange(0, q);

  e->coeffs[0] += p * k - sum(e);
  e->coeffs[0] %= q;
  return k;
}

static uint64_t generate_linear(uint64_t p, uint64_t q, uint64_t dim,
                                Polynomial *b) {
  uint64_t k = randrange(0, p);
  for (int i = 0; i < b->size; ++i)
    b->coeffs[i] = randrange(0, q);
  b->coeffs[0] += k - sum(b);
  b->coeffs[0] %= q;
  return k;
}

int aces_encrypt(const Aces *aces, const uint64_t *message, size_t size,
                 CipherMessage *result) {
  Polynomial r_m; // TODO: size aces->info.param.dim
  Polynomial e;
  Polynomial b;

  for (int i = 0; i < size; ++i) {
    generate_error(aces->info.channel.q, aces->info.param.dim, message[i],
                   &r_m);
    uint64_t k = generate_vanisher(aces->info.channel.p, aces->info.channel.q,
                                   aces->info.param.dim, &e);
    generate_linear(aces->info.channel.p, aces->info.channel.q,
                    aces->info.param.dim, &b);

    for (int j = 0; j < aces->info.param.dim; ++j) {
      poly_mul(&b, &(aces->privte_key.f0.polys[j]), &(result[i].c1.polys[j]),
               aces->info.channel.q);
      poly_mod(&(result[i].c1.polys[j]), &(aces->info.pk.u),
               &(result[i].c1.polys[j]), aces->info.channel.q);
    }

    // result[i].c2 = (r_m + b * (self.f1 + e)) % self.u; // TODO
    result[i].level = k * sum(b) % aces->info.channel.q;
  }

  return 0;
}

int aces_decrypt(const Aces *aces, const CipherMessage *message, size_t size,
                 uint64_t *result) {
  Polynomial c0Tx;
  Polynomial tmp;
  Polynomial m_pre;
  set_zero(&c0Tx);

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < aces->info.param.dim; ++j) {
      poly_mul(&(message[i].c1.polys[j]), &(aces->privte_key.x.polys[j]), &tmp,
               aces->info.channel.q);
      poly_add(&c0Tx, &tmp, &c0Tx, aces->info.channel.q);
    }

    poly_sub(&(message[i].c2), &c0Tx, &m_pre, aces->info.channel.q);
    result[i] = (sum(m_pre) % aces->info.channel.q) % aces->info.channel.p;
  }

  return 0;
}
