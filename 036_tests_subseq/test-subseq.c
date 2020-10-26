#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int *, size_t);

int main(void) {
  int array1[] = {};
  int array2[] = {1};
  int array3[] = {0, 0};
  int array4[] = {1, 2, 3, 1, 2, 2, 3};
  int array5[] = {-5, 5};
  int array6[] = {1, 2, 9999};
  int array7[] = {1, 2, 1, 3, 5, 7, 2, 4, 6, 9, 10};
  int array8[] = {-2147483648, 2147483647};

  assert(maxSeq(array1, 0) == 0);
  assert(maxSeq(array2, 1) == 1);
  assert(maxSeq(array3, 2) == 1);
  assert(maxSeq(array4, 7) == 3);
  assert(maxSeq(array5, 2) == 2);
  assert(maxSeq(array6, 3) == 3);
  assert(maxSeq(array7, 10) == 4);
  assert(maxSeq(array8, 2) == 2);

  return EXIT_SUCCESS;
}
