
int aces_add(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {
  for (int i = 0; i < dim; ++i) {
    poly_add(a->c1[i], b->c1[i], result->c1[i]);
    poly_mod(result->c1[i], u, result->c1[i]);
  }

  poly_add(a->c2, b->c2, result->c2);
  poly_mod(result->c2, u, result->c2);
  result->level = a->level + b->level;
}

int aces_mul(const CipherMessage *a, const CipherMessage *b,
             const SharedInfo *info, CipherMessage *result) {}

int aces_refresh(CipherMessage *a, uint64_t k) {
  poly_sub_scaler(a->c2, k);
  a->level -= k;
}
