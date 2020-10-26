#include "kv.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvpair_t parseLine(const char * line) {
  if (!line) {
    printf("The input is NULL");
    exit(EXIT_FAILURE);
  }

  kvpair_t kvpair;

  unsigned long line_len = strlen(line) - 1;
  unsigned long key_len = 0;
  unsigned long value_len = 0;

  // Check whether the input has ":", if not, the following operation would fail.
  if (!strchr(line, '=')) {
    printf("The input has no '='. The input format is invalid");
    exit(EXIT_FAILURE);
  }
  const char * p1 = strchr(line, '=');

  int count = 0;

  while (line[count] != '=') {
    count++;
  }
  key_len = count;
  value_len = line_len - 1 - key_len;

  ///strncmp
  char * key;
  char * value;

  key = strndup(line, key_len);
  value = strndup(p1 + 1, value_len);

  //  key[key_len] = '\0';
  //  value[value_len] = '\0';

  ///Check whether the input is valid:

  if ((key_len == 0) || (value_len == 0)) {
    printf("The input lack one of the following element: name,points,games.");
    exit(EXIT_FAILURE);
  }

  //Check whether the name is valid (letter or space)
  const char * space = " ";
  const char * ch = "'";
  const char * ch2 = ".";
  for (unsigned long i = 0; i < key_len; i++)
    if (!(isupper((int)key[i]) || islower((int)key[i]) || key[i] == *space ||
          key[i] == *ch || key[i] == *ch2)) {
      printf("key is invalid");
      exit(EXIT_FAILURE);
    }

  kvpair.key = key;
  kvpair.value = value;

  return kvpair;
}

kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  kvarray_t * kvarray;

  kvarray = malloc(sizeof(*kvarray));
  kvarray->numkv = 0;

  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    return NULL;
  }
  kvarray->kvpair = NULL;
  char * line = NULL;
  size_t size = 0;
  while (getline(&line, &size, f) != -1) {
    kvarray->numkv++;
    kvarray->kvpair =
        realloc(kvarray->kvpair, kvarray->numkv * sizeof(*(kvarray->kvpair)));
    kvarray->kvpair[kvarray->numkv - 1] = parseLine(line);
    free(line);
    line = NULL;
  }
  free(line);
  fclose(f);
  return kvarray;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->numkv; i++) {
    free(pairs->kvpair[i].key);
    free(pairs->kvpair[i].value);
  }

  free(pairs->kvpair);
  free(pairs);
}
void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->numkv; i++) {
    printf("key = '%s' value = '%s'\n", pairs->kvpair[i].key, pairs->kvpair[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->numkv; i++) {
    if (strcmp(pairs->kvpair[i].key, key) == 0) {
      return pairs->kvpair[i].value;
    }
  }
  return NULL;
}
