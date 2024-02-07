#include "Aces.h"

static int generate_error(uint64_t q, uint64_t dim, uint64_t message,
                          Polynomial *rm) {
  for (int i = 0; i < rm->size; ++i)
    rm->coeffs[i] = randrange(0, q);

  rm->coeffs[0] += m - sum(rm);
  rm->coeffs[0] %= q;
}

static ssize_t generate_vanisher(uint64_t p, uint64_t q, uint64_t dim,
                                 Polynomial *e) {
  uint64_t k = randrange(0, 1) < EPROB ? 0 : 1;
  for (int i = 0; i < rm->size; ++i)
    e->coeffs[i] = randrange(0, q);

  e->coeffs[0] += p * k - sum(e);
  e->coeffs[0] %= q;
}

static ssize_t generate_linear(uint64_t p, uint64_t q, uint64_t dim,
                               Polynomial *b) {
  uint64_t k = randrange(0, p);
  for (int i = 0; i < rm->size; ++i)
    b->coeffs[i] = randrange(0, q);
  b->coeffs[0] += k - sum(e);
  b->coeffs[0] %= q;
}

int aces_encrypt(const Aces *aces, const uint64_t *message, size_t size,
                 CipherMessage *result) {
  Polynomial r_m; // TODO: size aces->info.param.dim
  Polynomial e;
  Polynomial b;

  for (int i = 0; i < size; ++i) {
    generate_error(aces->info.channel.q, aces->info.param.dim, message[i], &rm);
    uint64_t k = generate_vanisher(aces->info.channel.p, aces->info.channel.q,
                                   aces->info.param.dim, &e);
    generate_linear(aces->info.channel.p, aces->info.channel.q,
                    aces->info.param.dim, &b);

    for (int j = 0; j < aces->info.param.dim; ++j) {
      poly_mul(b, aces->privte_key.f0[j], result[i].c1[j]);
      poly_mod(result[i].c1[j], aces->info.pk.u, result[i].c1[j]);
    }

    result[i].c2 = (r_m + b * (self.f1 + e)) % self.u; // TODO
    result[i].level = k * sum(b) % aces->info.channel.q;
  }
}

int aces_decrypt(const Aces *aces, const CipherMessage *message, size_t size,
                 uint64_t *result) {
  Polynomial c0Tx;
  Polynomial tmp;
  Polynomial m_pre;
  set_zero(c0Tx);

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < aces->info.param.dim; ++j) {
      poly_mul(message[i].c1[j], aces->privte_key.x[j], tmp);
      poly_add(c0Tx, tmp, c0Tx);
    }

    poly_sub(message[i].c2, c0Tx, m_pre);
    result[i] = (sum(m_pre) % q) % p;
  }

  return 0;
}
