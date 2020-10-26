#ifndef __CATARRAY_H__
#define __CATARRAY_H__
#include <stdlib.h>

struct category_tag {
  //Write code here for this struct
  char * category;
  char ** wordlist;
  size_t n_words;
};
typedef struct category_tag category_t;

struct catarray_tag {
  //Write code here for this struct
  category_t ** catlist;
  size_t n_cats;
};
typedef struct catarray_tag catarray_t;

catarray_t * readWords(FILE * f);
void addWord(const char * word,
             const char * ctgry,
             catarray_t * catarr);  // adds a word to a category in a catarray_t *
int catIndex(const char * ctgry,
             const catarray_t *
                 catarr);  // returns the index of a particular category in a catarray_t *
void addCat(const char * ctgry,
            catarray_t * catarr);  // adds a new category to a catarray_t *
void addWordToCat(const char * word,
                  const char * ctgry,
                  catarray_t * catarr);  // adds a new word to an existing category

void readStory(FILE * f, catarray_t * words);
char ** parseLine(FILE * fout,
                  char * line,
                  catarray_t * words,
                  char ** preWords,
                  int * numPreWords);  // handles one line read from the template file
char ** replaceBlank(
    FILE * fout,
    const char * token,
    catarray_t * words,
    char ** preWords,
    int * numPreWords);  // replaces a single blank with the appropriate word

void freeWords(catarray_t * words);

int randGen(catarray_t * cat, int index);
#endif
