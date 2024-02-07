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

int init_channel(Channel *channel, uint64_t p, uint64_t q, uint64_t w);

int generate_u(const Channel *channel, const Parameters *param, Polynomial *u);

int generate_secret(const Channel *channel, const Parameters *param,
                    const Polynomial *u, PolyArray *secret, Lambda *lambda);

int generate_f0(const Channel *channel, const Parameters *param, PolyArray *f0);

int generate_f1(const Channel *channel, const Parameters *param,
                const PolyArray *f0, const PolyArray *x, Polynomial *f1);

#ifdef __cplusplus
}
#endif

#endif
