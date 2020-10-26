//This file is for Step 2.
//You should do
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sum_seq2
//  Step 2 (D): add test cases to main to test sum_seq2
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq2(int x) {
  return (x - 2) * (x - 2) - 2;
}

int sum_seq2(int low, int high) {
  int num = high - low;
  if (high <= low) {
    return 0;
  }
  else {
    int sum = 0;
    for (int i = 0; i < num; i++) {
      sum = sum + seq2(low + i);
    }
    return sum;
  }
}

int main(void) {
  printf("seq2(%d)=%d\n", -1, seq2(-1));
  printf("seq2(%d)=%d\n", 1, seq2(1));
  printf("seq2(%d)=%d\n", 0, seq2(0));
  printf("seq2(%d)=%d\n", 999999, seq2(999999));
  printf("seq2(%d)=%d\n", -999999, seq2(-999999));

  printf("sum_seq2(%d,%d)=%d\n", -4, -1, sum_seq2(-4, -1));
  printf("sum_seq2(%d,%d)=%d\n", 1, 4, sum_seq2(1, 4));
  printf("sum_seq2(%d,%d)=%d\n", -1, -4, sum_seq2(-1, -4));
  printf("sum_seq2(%d,%d)=%d\n", 4, 1, sum_seq2(4, 1));
  printf("sum_seq2(%d,%d)=%d\n", 0, 2, sum_seq2(0, 2));
  printf("sum_seq2(%d,%d)=%d\n", -2, 0, sum_seq2(-2, 0));
  printf("sum_seq2(%d,%d)=%d\n", 99995, 99999, sum_seq2(99995, 99999));
  printf("sum_seq2(%d,%d)=%d\n", -99999, -99995, sum_seq2(-99999, -99995));
  printf("sum_seq2(%d,%d)=%d\n", 0, 0, sum_seq2(0, 0));

  return EXIT_SUCCESS;
}
