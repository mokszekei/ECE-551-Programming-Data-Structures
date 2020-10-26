#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int *, size_t);

int main(void) {
  int array1[] = {0};
  int array2[] = {1};
  int array3[] = {0, 0};
  int array4[] = {1, 2, 3, 1, 2, 2, 3};
  int array5[] = {-5, 5};
  int array6[] = {1, 2, 9999};

  assert(maxSeq(array1, 1) == 1);
  assert(maxSeq(array2, 1) == 1);
  assert(maxSeq(array3, 2) == 1);
  assert(maxSeq(array4, 7) == 3);
  assert(maxSeq(array5, 2) == 2);
  assert(maxSeq(array6, 3) == 3);

  return EXIT_SUCCESS;
}

size_t maxSeq(int * array, size_t n) {
  int count = 1;
  int result = 1;
  for (size_t i = 0; i < n; i++) {
    if (array[i + 1] > array[i]) {
      count++;
    }
    else {
      if (count > result) {
        result = count;
      }
      count = 0;
    }
  }
  return result;
}
