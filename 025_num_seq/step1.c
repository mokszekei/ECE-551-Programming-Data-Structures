//This file is for Step 1.
//You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write print_seq1_range
//  Step 1 (D): add test cases to main to test print_seq1_range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>

int seq1(int x) {
  return 3 * x - 5;
}

void print_seq1_range(int low, int high) {
  if (low >= high) {
    printf("\n");
  }
  else {
    int num = high - low;
    for (int i = 0; i < num; i++) {
      printf("%d", seq1(low + i));
      if (i + 1 != num) {
        printf(", ");
      }
      else {
        printf("\n");
      }
    }
  }
}

int main(void) {
  printf("seq1(%d)=%d\n", -1, seq1(-1));
  printf("seq1(%d)=%d\n", 1, seq1(1));
  printf("seq1(%d)=%d\n", 0, seq1(0));
  printf("seq1(%d)=%d\n", 999999, seq1(999999));
  printf("seq1(%d)=%d\n", -999999, seq1(-999999));

  printf("print_seq1_range(%d,%d)\n", 0, 0);
  print_seq1_range(0, 0);
  printf("print_seq1_range(%d,%d)\n", 0, 4);
  print_seq1_range(0, 4);
  printf("print_seq1_range(%d,%d)\n", -2, 0);
  print_seq1_range(-2, 0);
  printf("print_seq1_range(%d,%d)\n", -2, 6);
  print_seq1_range(-2, 6);
  printf("print_seq1_range(%d,%d)\n", -3, -1);
  print_seq1_range(-3, -1);
  printf("print_seq1_range(%d,%d)\n", 1, 3);
  print_seq1_range(1, 3);
  printf("print_seq1_range(%d,%d)\n", 7, 3);
  print_seq1_range(7, 3);
  printf("print_seq1_range(%d,%d)\n", -1, -3);
  print_seq1_range(-1, -3);
  printf("print_seq1_range(%d,%d)\n", 1000000, 1000005);
  print_seq1_range(1000000, 1000005);
  printf("print_seq1_range(%d,%d)\n", -1000005, -1000000);
  print_seq1_range(-1000005, -1000000);

  return EXIT_SUCCESS;
}
