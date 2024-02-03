#ifndef OPERATIONS_H
#define OPERATIONS_H

int aces_add(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result);

int aces_mul(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result);

int aces_refresh(CipherMessage *a, uint64_t k);

#ifdef __cplusplus
}
#endif

#endif
