#include <Aces.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 10
#define MEMORY_SIZE 2 << 14

int main() {
  srand(6);
  uint8_t memory[MEMORY_SIZE];

  Aces aces;
  set_aces(&aces, DIM, memory, MEMORY_SIZE);
  init_aces(2, 33, DIM, &aces);

  uint64_t message = 1;
  CipherMessage c_msg;

  Coeff c1_mem[DIM][DIM];
  Polynomial c1_polies[DIM];
  for (int i = 0; i < DIM; ++i) {
    set_polynomial(&c1_polies[i], c1_mem[i], DIM);
  }
  c_msg.c1.size = DIM;
  c_msg.c1.polies = c1_polies;

  Coeff c2_mem[DIM];
  set_polynomial(&c_msg.c2, c2_mem, DIM);

  aces_encrypt(&aces, &message, 1, &c_msg);

  uint64_t recovered_message;
  aces_decrypt(&aces, &c_msg, 1, &recovered_message);

  printf("%lu == %lu\n", recovered_message, message % 2);

  return 0;
}
