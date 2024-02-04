#ifndef CHANNEL_H
#define CHANNEL_H

#include "Polynomial.h"

typename struct {
  uint64_t dim;
  uint64_t N;
} Parameters;

typename struct {
  uint64_t p;
  uint64_t q;
  uint64_t w;
} Channel;

typename struct {
  uint64_t ***lambda;
  size_t size[3];
} Lambda;

int init_channel(Channel *channel, uint64_t p, uint64_t q, uint64_t w) {
  channel->w = w;
  channel->p = p;
  channel->q = q;

  if (!(pow(p, 2) < q && are_coprime(p, q)))
    channel->q = pow(p + 1, 2);

  return 0;
}

int generate_u(const Channel *channel, const Parameters *param, Polynomial *u) {
  uint64_t nonzeros = clamp(param->dim / 2, param->dim - 1,
                            normal_rand(param->dim / 2, param->dim / 2));
  uint64_t zeroes = param->dim - nonzeros;
  u->coeffs[0] = 1;

  for (int i = 0; i < param->dim - 1; ++i) {
    if (nonzeros > 1) {
      u->coeffs[i] = randrange(0, param->q);
      nonzeros--;
    }

    if (zeroes > 0) {
      uint64_t samp = clamp(0, zeroes, normal_rand(zeroes / 2, zeroes / 2));
      uint64_t zero = randrange(0, samp);
      for (int j = 0; j < zero; ++j)
        u->coeffs[i++] = 0;
      zeroes -= zero;
    }
  }

  u->coeffs[param->dim - 1] = 0;
  u->coeffs[param->dim - 1] = param->q - sum(u);
}

int generate_secret(const Channel *channel, const Parameters *param,
                    const Polynomial *u, PolyArray *secret, Lambda *lambda);

int generate_f0(const Channel *channel, const Parameters *param, PolyArray *f0);

int generate_f1(const Channel *channel, const Parameters *param,
                const Polynomial *, Polynomial *f1);

#ifdef __cplusplus
}
#endif

#endif
