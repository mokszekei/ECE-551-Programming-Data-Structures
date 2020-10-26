//This file is for Step 3.
//You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write count_neg_seq3_range
//  Step 3 (D): add test cases to main to test count_neg_seq3_range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq3(int x, int y) {
  return 3 + (x - 1) * (y + 4);
}

int count_neg_seq3_range(int xLow, int xHi, int yLow, int yHi) {
  if (xLow < xHi && yLow < yHi) {
    int count = 0;
    int nx = xHi - xLow;
    int ny = yHi - yLow;
    for (int i = 0; i < nx; i++) {
      for (int j = 0; j < ny; j++) {
        if (seq3(xLow + i, yLow + j) < 0) {
          count++;
        }
      }
    }
    return count;
  }
  else {
    return 0;
  }
}

int main(void) {
  printf("seq3(%d,%d)=%d\n", -1, -2, seq3(-1, -2));
  printf("seq3(%d,%d)=%d\n", 2, 1, seq3(2, 1));
  printf("seq3(%d,%d)=%d\n", 0, 0, seq3(0, 0));
  printf("seq3(%d,%d)=%d\n", 99999, 99999, seq3(99999, 99999));
  printf("seq3(%d,%d)=%d\n", -99999, -99999, seq3(-99999, -99999));
  printf("seq3(%d,%d)=%d\n", 0, 1, seq3(0, 1));
  printf("seq3(%d,%d)=%d\n", -1, 0, seq3(-1, 0));
  printf("seq3(%d,%d)=%d\n", -2, -1, seq3(-2, -1));

  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         0,
         3,
         count_neg_seq3_range(0, 2, 0, 3));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         -2,
         0,
         3,
         count_neg_seq3_range(0, -2, 0, 3));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         0,
         -3,
         count_neg_seq3_range(0, 2, 0, -3));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         0,
         0,
         3,
         count_neg_seq3_range(0, 0, 0, 3));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         2,
         0,
         0,
         count_neg_seq3_range(0, 2, 0, 0));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         -2,
         0,
         -3,
         0,
         count_neg_seq3_range(-2, 0, -3, 0));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         -2,
         0,
         0,
         3,
         count_neg_seq3_range(-2, 0, 0, 3));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         0,
         4,
         -3,
         0,
         count_neg_seq3_range(0, 4, -3, 0));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         1,
         1,
         1,
         1,
         count_neg_seq3_range(1, 1, 1, 1));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         -1,
         2,
         -1,
         3,
         count_neg_seq3_range(-1, 2, -1, 3));

  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         -1,
         9999,
         -1,
         9999,
         count_neg_seq3_range(-1, 9999, -1, 9999));
  printf("count_neg_seq3_range(%d, %d, %d, %d) = %d\n",
         -9999,
         1,
         -9999,
         1,
         count_neg_seq3_range(-9999, 1, -9999, 1));

  return EXIT_SUCCESS;
}
