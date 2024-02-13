#include "channel-test.h"

#include <Channel.h>
#include <Polynomial.h>

#include <assert.h>
#include <stdio.h>

int generate_u_test() {
  Polynomial u;
  Coeff mem[11];
  set_polynomial(&u, mem, 11);
  set_zero(&u);

  Channel channel;
  init_channel(&channel, 2, 33, 1);

  Parameters param;
  param.dim = 10;

  generate_u(&channel, &param, &u);
  assert(coef_sum(&u) == 33);

  return 1;
}

int generate_f0_test() {
  Polynomial polies[10];

  PolyArray f0;
  f0.size = 10;
  f0.polies = polies;

  Coeff mem[10][10];
  for (int i = 0; i < 10; ++i)
    set_polynomial(&polies[i], mem[i], 10);

  Channel channel;
  init_channel(&channel, 2, 33, 1);
  Parameters param;
  param.dim = 10;

  generate_f0(&channel, &param, &f0);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%ld\t", f0.polies[i].coeffs[j]);
    }
    printf("\n");
  }

  return 1;
}

int run_channel_tests() {
  assert(generate_u_test());
  assert(generate_f0_test());
  return 1;
}
