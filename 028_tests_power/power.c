
#include <stdio.h>
#include <stdlib.h>

int helper(int x, int y, int answer);

unsigned power(unsigned x, unsigned y) {
  if (x == 0 && y == 0) {
    return 0;
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
  printf("%d,%d,%d", 2, 3, power(2, 3));
  printf("%d,%d,%d", 3, 3, power(3, 3));
}
