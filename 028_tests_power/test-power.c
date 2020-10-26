#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    printf("The function failed to compute power(%d,%d)", x, y);
    exit(EXIT_FAILURE);
  }
}

int helper(int x, int y, int answer);

unsigned power(unsigned x, unsigned y) {
  if (x == 0 && y == 0) {
    return 1;
  }
  else {
    return helper(x, y, x);
  }
}

int helper(int x, int y, int answer) {
  if (y == 1) {
    return answer;
  }
  else {
    return helper(x, y - 1, answer * x);
  }
}

int main(void) {
  run_check(0, 0, 1);
  run_check(0, 1, 0);
  run_check(1, 0, 1);
  run_check(2, 0, 1);
  run_check(3, 3, 27);
  run_check(5, 4, 625);
  run_check(342424, 1, 342424);
  return EXIT_SUCCESS;
}
