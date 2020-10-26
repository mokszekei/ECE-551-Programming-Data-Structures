#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int maxindex(int * array, int size) {
  int max = 0;
  for (int i = 0; i < size; i++) {
    if (array[i] > array[max]) {
      max = i;
    }
  }
  return max;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage %s file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "failed to open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  int c;
  int array[26];
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      array[tolower(c) - 'a']++;
    }
  }
  int index = maxindex(array, 26);
  int key = (index + 22) % 26;
  printf("%d\n", key);

  fclose(f);
  return EXIT_SUCCESS;
}
