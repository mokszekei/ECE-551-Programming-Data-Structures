#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t count = 1;
  size_t result = 1;
  if (n <= 0) {
    return 0;
  }
  else {
    for (size_t i = 0; i < n; i++) {
      if (array[i + 1] > array[i]) {
        count++;
      }
      else {
        if (count > result) {
          result = count;
        }
        count = 1;
      }
    }
    return result;
  }
}
