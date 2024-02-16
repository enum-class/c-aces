#ifndef CHANNEL_H
#define CHANNEL_H

#include "Common.h"
#include "Polynomial.h"

typedef struct {
  uint64_t dim;
  uint64_t N;
} Parameters;

typedef struct {
  uint64_t p;
  uint64_t q;
  uint64_t w;
} Channel;

typedef struct {
  Matrix2D *data;
  size_t size;
} Lambda;

Matrix2D *lambda_get(Lambda *lambda, size_t idx);

int init_channel(Channel *, uint64_t, uint64_t, uint64_t);

int generate_u(const Channel *, const Parameters *, Polynomial *);

int generate_secret(const Channel *, const Parameters *, const Polynomial *,
                    PolyArray *, Lambda *);

int generate_f0(const Channel *, const Parameters *, PolyArray *);

int generate_f1(const Channel *, const Parameters *, const PolyArray *,
                const PolyArray *, Polynomial *);

#ifdef __cplusplus
}
#endif

#endif
