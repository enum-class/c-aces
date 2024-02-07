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
  nonzeros--;

  for (int i = 1; i < param->dim - 1;) {
    if (nonzeros > 1) {
      u->coeffs[i++] = randrange(0, param->q);
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
  return 0;
}

int generate_secret(const Channel *channel, const Parameters *param,
                    const Polynomial *u, PolyArray *secret, Lambda *lambda) {
  uint64_t m[param.dim][param.dim];
  uint64_t inv_m[param.dim][param.dim];
  fill_inv_random(m, inv_m, param.dim);

  for (int i = 0; i < secret->size; ++i) {
    for (int j = 0; j < secret[i]->size; ++j) {
      secret[i][j] = m[j][i];
    }
  }

  // TODO:
}

int generate_f0(const Channel *channel, const Parameters *param,
                PolyArray *f0) {
  for (int i = 0; i < f0->size; ++i) {
    for (int j = 0; j < f0[i]->size; j++)
      f0[i]->coeffs[j] = q1 * randrange(0, channel->q) % channel->q;

    int shift_idx = randrange(0, f0[i]->size);
    f0[i]->coeffs[shift_idx] =
        (f0[i]->coeffs[shift_idx] - sum(f0[i])) % channel->q;
  }

  return 0;
}

int generate_f1(const Channel *channel, const Parameters *param,
                const PolyArray *f0, const PolyArray *x, Polynomial *f1) {
  set_zero(f1);
  for (int i = 0; i < param->dim; ++i)
    poly_mul(f0[i], x[i], f1, channel->q);

  poly_mod(f1, channel->u, f1, channel->q);
  return 0;
}
