#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Aces.h"

int aces_add(const CipherMessage *, const CipherMessage *, const SharedInfo *,
             CipherMessage *);

int aces_mul(const CipherMessage *, const CipherMessage *, const SharedInfo *,
             CipherMessage *);

int aces_refresh(CipherMessage *, uint64_t);

#ifdef __cplusplus
}
#endif

#endif
