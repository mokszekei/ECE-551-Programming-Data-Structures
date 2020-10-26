

#include "counts.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t * createCounts(void) {
  //WRITE ME
  counts_t * c;
  c = malloc(sizeof(*c));
  c->count = NULL;
  c->n = 0;
  c->unknown = 0;
  return c;
}

void addCount(counts_t * c, const char * name) {
  //WRITE ME
  assert(c != NULL);
  if (name == NULL) {
    c->unknown++;
    return;
  }
  int match = 0;
  for (size_t i = 0; i < c->n; i++) {
    if (strncmp(c->count[i].key, name, strlen(name)) == 0) {
      c->count[i].one_count++;
      match++;
    }
  }
  if (match == 0) {
    c->n++;
    c->count = realloc(c->count, c->n * sizeof(*c->count));
    c->count[c->n - 1].key = strdup(name);
    c->count[c->n - 1].one_count = 1;
  }
  if (c->count[c->n - 1].key == NULL) {
    exit(EXIT_FAILURE);
  }
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (size_t i = 0; i < c->n; i++) {
    fprintf(outFile, "%s: %d\n", c->count[i].key, c->count[i].one_count);
  }
  if (c->unknown != 0) {
    fprintf(outFile, "<unknown>: %d", c->unknown);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (size_t i = 0; i < c->n; i++) {
    free(c->count[i].key);
  }
  free(c->count);
  free(c);
}
