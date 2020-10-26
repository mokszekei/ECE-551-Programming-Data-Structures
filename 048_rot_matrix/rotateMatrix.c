#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 12

void rot(char matrix[10][10]) {
  char m90[10][10];
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      m90[j][9 - i] = matrix[i][j];
    }
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%c", m90[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char ** argv) {
  char matrix[10][10];
  char line[LINE_SIZE];
  if (argc != 2) {
    fprintf(stderr, "Usage: rotateMatrix input\n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");

  if (f == NULL) {
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (fgets(line, LINE_SIZE, f) != NULL) {
    if (strchr(line, '\n') == NULL) {
      printf("Line is too long.");
      exit(EXIT_FAILURE);
    }
    strncpy(matrix[i], line, 10);
    i++;
  }
  rot(matrix);
}
