
#include <stdio.h>
#include <stdlib.h>

int helper(int x, int y, int answer);

unsigned power(unsigned x, unsigned y) {
  if (y == 0) {
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
