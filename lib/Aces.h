#ifndef ACES_H
#define ACES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Channel.h"
#include "Polynomial.h"

#define EPROB 0.5

typename struct {
  Polynomial u;
  Lambda lambda;
} PublicKey;

typename struct {
  PolyArray x;
  PolyArray f0;
  Polynomial f1;
} PrivateKey;

typename struct {
  Channel channel;
  Parameters param;
  PublicKey pk;
} SharedInfo;

typename struct {
  SharedInfo info;
  PrivateKey privte_key;
} Aces;

typename struct {
  PolyArray c1;
  Polynomial c2;
  uint64_t level;
} CipherMessage;

int aces_encrypt(const Aces *aces, const uint64_t *message, size_t size,
                 CipherMessage *result);

int aces_decrypt(const Aces *aces, const CipherMessage *message, size_t size,
                 uint64_t *message);

#ifdef __cplusplus
}
#endif

#endif
