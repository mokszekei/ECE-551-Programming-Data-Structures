#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void doeverything(FILE * f) {
  char ** array = NULL;
  char * line = NULL;
  size_t size = 0;
  size_t i = 0;
  while (getline(&line, &size, f) >= 0) {
    array = realloc(array, (i + 1) * sizeof(*array));
    array[i] = line;
    line = NULL;
    i++;
  }
  free(line);
  sortData(array, i);
  for (size_t j = 0; j < i; j++) {
    printf("%s", array[j]);
    free(array[j]);
  }
  free(array);
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!

  ///If no arguments (other than its own name) are provided (argc ==1), then
  /// your program should read from standard input, sort the lines of input,
  ///  print the results, free memory, and exit successfully.
  //if (argc==){
  //  FILE * f =fopen(stdin,"r");
  //}

  if (argc >= 1) {
    for (int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      doeverything(f);
    }
  }
  return EXIT_SUCCESS;
}
