#ifndef ACES_H
#define ACES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Channel.h"
#include "Polynomial.h"

#define EPROB 0.5

typedef struct {
  Polynomial u;
  Lambda lambda;
} PublicKey;

typedef struct {
  PolyArray x;
  PolyArray f0;
  Polynomial f1;
} PrivateKey;

typedef struct {
  Channel channel;
  Parameters param;
  PublicKey pk;
} SharedInfo;

typedef struct {
  SharedInfo info;
  PrivateKey privte_key;
} Aces;

typedef struct {
  PolyArray c1;
  Polynomial c2;
  uint64_t level;
} CipherMessage;

int aces_encrypt(const Aces *, const uint64_t *, size_t, CipherMessage *);

int aces_decrypt(const Aces *, const CipherMessage *, size_t size, uint64_t *);

#ifdef __cplusplus
}
#endif

#endif
