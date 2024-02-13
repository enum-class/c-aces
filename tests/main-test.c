#include "channel-test.h"
#include "polynomial-test.h"

#include <assert.h>

int main(void) {
  assert(run_polynomial_tests());
  assert(run_channel_tests());
  return 0;
}
