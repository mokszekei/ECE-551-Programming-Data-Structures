/*** Some summary
Fitst, I use char ** to represent the previous words. I realized that I should use a struct like category_tag to represent it,
because it cause a lot of problems when I try to free the memory. But it's too late to modify the code, so I 
use it as the return value of the two functions. I know it's really not good design.

Second, some of my code have lots of repeatitions. I should use some alias to simplify it to increase readability.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "catarray.h"
#define SET 10

int main(void) {
  char storyfilename[] = "story.txt";
  char wordfilename[] = "words.txt";
  int seed = 1;
  scanf("%s%s%d", storyfilename, wordfilename, &seed);
  //  printf("%s %s %d\n", storyfilename, wordfilename, seed);

  srand(seed);
  FILE * wordfile;
  catarray_t * catarray = NULL;
  if ((wordfile = fopen(wordfilename, "r")) != NULL) {
    catarray = readWords(wordfile);
  }
  else {
    printf("Can't open words file.\n");
    exit(EXIT_FAILURE);
  }
  fclose(wordfile);

  FILE * storyfile;
  if ((storyfile = fopen("story.txt", "r")) != NULL) {
    readStory(storyfile, catarray);
  }
  else {
    printf("Can't open story file.\n");
    exit(EXIT_FAILURE);
  }

  fclose(storyfile);

  freeWords(catarray);
  //  free(storyfilename);
  //  free(wordfilename);
  return 0;
}

void freeWords(catarray_t * words) {
  int i = 0;
  for (; i < words->n_cats; i++) {
    int j = 0;
    for (; j < (words->catlist)[i]->n_words; j++) {
      free(((words->catlist)[i]->wordlist)[j]);
    }
    free((words->catlist)[i]->wordlist);
    free((words->catlist)[i]->category);
    free((words->catlist)[i]);
  }
  free(words->catlist);
  free(words);
  return;
}

void readStory(FILE * f, catarray_t * words) {
  FILE * out = fopen("output.txt", "w");

  ssize_t read;
  char * line = NULL;
  size_t len = 0;

  char ** preWords = (char **)malloc(sizeof(char *) * SET);
  int numPreWords = 0;

  while ((read = getline(&line, &len, f)) != -1) {
    char * temp = strdup(line);
    preWords = parseLine(out, temp, words, preWords, &numPreWords);
    free(temp);
  }
  free(line);

  int i = 0;
  for (i = 0; i < numPreWords; i++) {
    free(preWords[i]);
  }
  free(preWords);
  fclose(out);
  return;
}

char ** parseLine(FILE * fout,
                  char * line,
                  catarray_t * words,
                  char ** preWords,
                  int * numPreWords) {  // handles one line read from the template file
  char delim[] = "_\n";

  fputs(strsep(&line, delim), fout);
  char * token = strsep(&line, delim);
  while (line) {
    preWords = replaceBlank(fout, token, words, preWords, numPreWords);

    fputs(strsep(&line, delim), fout);
    token = strsep(&line, delim);
  }
  fputs("\n", fout);
  return preWords;
}

char ** replaceBlank(FILE * fout,
                     const char * token,
                     catarray_t * words,
                     char ** preWords,
                     int * numPreWords) {
  int index = catIndex(token, words);
  char * word;

  if (index != -1) {
    int ran = randGen(words, index);
    word = strdup(((words->catlist)[index]->wordlist)[ran]);
  }
  else {
    word = strdup(preWords[*numPreWords - atoi(token)]);
  }
  if (*numPreWords == SET) {
    preWords = (char **)realloc(preWords, (*numPreWords + SET) * sizeof(char *));
  }
  preWords[*numPreWords] = strdup(word);
  *numPreWords += 1;
  fputs(word, fout);
  free(word);

  return preWords;
}  // replaces a single blank with the appropriate word

int randGen(catarray_t * cat, int index) {
  int nwords = (cat->catlist)[index]->n_words;
  //  srand(time(NULL));
  int ran = rand() % nwords;
  return ran;
}

catarray_t * readWords(FILE * f) {
  catarray_t * res = (catarray_t *)malloc(sizeof(catarray_t));
  if (res) {
    res->catlist = (category_t **)malloc(sizeof(category_t *));
    res->n_cats = 0;
  }
  else {
    printf("Can't malloc catarray_t.\n");
    exit(EXIT_FAILURE);
  }

  ssize_t read;
  char * line = NULL;
  size_t len = 0;
  while ((read = getline(&line, &len, f)) != -1) {
    char delim[] = ":\n";
    char * token;
    char * s = strdup(line);
    char * t = s;  // to remember the beginning of s, to free it later.
    token = strsep(&s, delim);
    char * cat = strdup(token);
    token = strsep(&s, delim);
    char * wd = strdup(token);
    free(t);

    addWord(wd, cat, res);
    free(cat);
    free(wd);
  }
  free(line);

  return res;
}

int catIndex(const char * ctgry, const catarray_t * catarr) {
  int i = 0;
  for (; i < catarr->n_cats; i++) {
    if (strcmp(ctgry, (catarr->catlist)[i]->category) == 0) {
      return i;
    }
  }
  return -1;
}

void addCat(const char * ctgry, catarray_t * catarr) {
  catarr->n_cats += 1;

  catarr->catlist =
      (category_t **)realloc(catarr->catlist, catarr->n_cats * sizeof(category_t *));
  catarr->catlist[catarr->n_cats - 1] = (category_t *)malloc(sizeof(category_t));
  ((catarr->catlist)[catarr->n_cats - 1])->category =
      strdup(ctgry);  // here is a problem, really don't know where and how.
  ((catarr->catlist)[catarr->n_cats - 1])->n_words = 0;
  ((catarr->catlist)[catarr->n_cats - 1])->wordlist = (char **)malloc(sizeof(char *));

  return;
}

void addWordToCat(const char * word, const char * ctgry, catarray_t * catarr) {
  int index = catIndex(ctgry, catarr);
  category_t * cat = (catarr->catlist)[index];
  cat->n_words += 1;
  cat->wordlist = (char **)realloc(cat->wordlist, cat->n_words * sizeof(char *));
  cat->wordlist[cat->n_words - 1] = strdup(word);

  return;
}

void addWord(const char * word, const char * ctgry, catarray_t * catarr) {
  int index = catIndex(ctgry, catarr);
  if (index == -1) {
    addCat(ctgry, catarr);
  }
  addWordToCat(word, ctgry, catarr);
  return;
}
