#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "retirement.h"

//Error Function
void check_line(char * line, int standard) {
  int n = strlen(line);
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (line[i] == ',') {
      count++;
    }
  }
  if (count != standard) {
    fprintf(stderr, "sp_line format incorrect!\n");
    exit(EXIT_FAILURE);
  }
}

//Helper Function
sp_t * sp_readline(char * line) {
  // CHECKING
  check_line(line, 2);
  // MALLOC
  char * temp = strdup(line);
  sp_t * ans = malloc(sizeof(*ans));
  // PARSING

  //char * year_str = line;
  //char * month_str = strchr(line, '.');
  //*month_str = '\0';
  //month_str++;
  //char * level_index_str = strchr(month_str, ',');
  char * dates_str = temp;
  char * level_index_str = strchr(dates_str, ',');
  *level_index_str = '\0';
  level_index_str++;
  char * dividend_str = strchr(level_index_str, ',');
  *dividend_str = '\0';
  dividend_str++;
  char * end_str = strchr(dividend_str, '\n');
  *end_str = '\0';
  // STR TO NUM

  //int year_int = atoi(year_str);
  //int month_int = atoi(month_str);
  double dates_f = atof(dates_str);
  double level_index_f = atof(level_index_str);
  double dividend_f = atof(dividend_str);
  if (dates_f == 0.0 || level_index_f == 0.0 || dividend_f == 0.0) {
    fprintf(stderr, "something other than number appears!\n");
    exit(EXIT_FAILURE);
  }
  //ASSIGNING TO STRUCT

  //ans->year = year_int;
  //ans->month = month_int;
  ans->dates = dates_f;
  ans->level_index = level_index_f;
  ans->dividend = dividend_f;
  free(temp);
  return ans;
}

bond_t * bond_readline(char * line) {
  // CHECKING
  check_line(line, 1);
  // MALLOC
  char * temp = strdup(line);
  bond_t * ans = malloc(sizeof(*ans));
  // PARSING

  //char * year_str = line;
  //char * month_str = strchr(line, '.');
  //*month_str = '\0';
  //month_str++;
  char * dates_str = temp;
  char * price_str = strchr(dates_str, ',');
  *price_str = '\0';
  price_str++;
  char * end_str = strchr(price_str, '\n');
  *end_str = '\0';
  // STR TO NUM

  //int year_int = atoi(year_str);
  //int month_int = atoi(month_str);
  double dates_f = atof(dates_str);
  double price_f = atof(price_str);
  if (dates_f == 0.0 || price_f == 0.0) {
    fprintf(stderr, "something other than number appears!\n");
    exit(EXIT_FAILURE);
  }
  // ASSIGNING TO STRUCT

  //ans->year = year_int;
  //ans->month = month_int;
  ans->dates = dates_f;
  ans->price = price_f;
  free(temp);
  return ans;
}

void free_sp(sp_t * array) {
  free(array);
}

void free_bond(bond_t * array) {
  free(array);
}

//double rorFromLine(char * line, double * tool) {
//BOND
//if (*tool == 12.00) {
//bond_t * bond_ans = bond_readline(line);
//  double bond_ror = (bond_ans->price / *tool);
//  return bond_ror;
//}
//SP
//  else {
//sp_t * sp_ans = sp_readline(line);
//  double sp_ror =
//      (sp_ans->level_index / *tool) - 1 + (sp_ans->dividend / 12 / sp_ans->level_index);
//  return sp_ror;
//}
//};

double sp_rorFromline(char * line, double * tool) {
  sp_t * sp_ans = sp_readline(line);
  //printf("the number of index_level last time: %f\n", *tool);
  double sp_ror =
      (sp_ans->level_index / *tool) - 1 + (sp_ans->dividend / 12 / sp_ans->level_index);
  free(sp_ans);
  return sp_ror;
}

double bond_rorFromline(char * line, double * tool) {
  bond_t * bond_ans = bond_readline(line);
  double bond_ror = (bond_ans->price / *tool) / 100;
  free(bond_ans);
  return bond_ror;
}

double read_dates(char * line) {
  char * temp = strdup(line);
  char * ans = temp;
  char * end = strchr(ans, ',');
  *end = '\0';
  //printf("%s\n", ans);
  double ans_f = atof(ans);
  //printf("%f\n", ans_f);
  //printf("current line date is: %f\n\n", ans_f);
  free(temp);
  return ans_f;
}

int check_file(FILE * f) {
  int c;
  int check = 0;
  int count = 0;
  while ((c = fgetc(f) != '\n')) {
    if (c == ',') {
      count++;
    }
    //SP = 1, BOND = 0
    if (count == 2) {
      check = 1;
    }
    else if (count == 1) {
      check = 0;
    }
  }
  return check;
}

int check_stra(char * line) {
  int count = 0;
  int check = 0;
  int n = strlen(line);
  for (int i = 0; i < n; i++) {
    if (line[i] == ',') {
      count++;
    }
  }
  //SP=2, BOND=1
  if (count == 2) {
    check = 2;
  }
  else if (count == 1) {
    check = 1;
  }
  //printf("check is: %d\n", check);
  return check;
}

void operation(ror_arr_t * ans,
               int * count_size,
               char * line,
               double * base,
               double (*rorFromLine)(char *, double *)) {
  ans->n++;
  ans->dates = realloc(ans->dates, *count_size * sizeof(*ans->dates));
  ans->ror = realloc(ans->ror, *count_size * sizeof(*ans->ror));
  ans->dates[*count_size - 1] = read_dates(line);
  //if (check_stra(line) == 1) {
  //ans->ror[*count_size - 1] = rorFromLine(line);
  //}
  //else if (check_stra(line) == 0) {
  //ans->ror[*count_size - 1] = rorFromLine(line);
  ans->ror[*count_size - 1] = rorFromLine(line, base);
}

// READFILE
ror_arr_t * readFile(FILE * f,
                     double start,
                     double end,
                     double (*rorFromLine)(char *, double *)) {
  double base = 0;
  //FOR GETLINE
  size_t sz = 0;
  size_t len = 0;
  char * line = NULL;
  //FOR MALLOC
  ror_arr_t * ans = malloc(sizeof(*ans));
  ans->n = 0;
  ans->dates = NULL;
  ans->ror = NULL;
  int count_size = 0;
  int button = 0;
  int skip = 0;
  double dates_read = 0;
  while ((len = getline(&line, &sz, f)) >= (size_t)0) {
    //printf("printing current line: %s\n", line);
    //printf("printing start and end: %f, %f \n", start, end);
    if (skip == 0) {
      //printf("Skip successful\n");
      skip = 1;
      //printf("skip = %d\n\n", skip);
      continue;
    }
    dates_read = read_dates(line);
    //printf("Current iteration date: %f\n\n", dates_read);
    //printf("Just the check: %f\n\n", dates_read - end);
    //if (dates_read == end) {
    if (-(dates_read - end) <= 0.001) {
      operation(ans, &count_size, line, &base, rorFromLine);
      count_size++;
      //printf("Successfully reach the end\n");
      //printf("This is the end, and the date is %f\n", dates_read);
      button = 0;
      //operation(ans, &count_size, line, &base, rorFromLine);
      //printf("printing dates: %f\n", ans->dates[count_size - 1]);
      if (check_stra(line) == 2) {
        sp_t * temp_temp = sp_readline(line);
        base = temp_temp->level_index;
        free(temp_temp);
        //base = sp_readline(line)->level_index;
        //printf("base is: %f\n", base);
      }
      else if (check_stra(line) == 1) {
        base = 12;
      }
      //printf("printing ror: %f\n\n", ans->ror[count_size - 1]);
      break;
    }
    if (button == 1) {
      operation(ans, &count_size, line, &base, rorFromLine);
      count_size++;
      //printf("Button = 1\n");
      //operation(ans, &count_size, line, &base, rorFromLine);
      //printf("printing current value of n: %d\n", (int)ans->n);
      if (check_stra(line) == 2) {
        sp_t * temp_temp = sp_readline(line);
        base = temp_temp->level_index;
        free(temp_temp);
        //base = sp_readline(line)->level_index;
      }
      else if (check_stra(line) == 1) {
        base = 12;
      }
      //printf("printing current ror: %f\n\n", ans->ror[count_size - 1]);
      continue;
    }
    //if (read_dates(line) != start) {
    if (dates_read != start) {
      //printf("going to next iteration!\n");
      //printf("%f\n", read_dates(line));
      continue;
    }
    //else if (dates_read == start) {
    if (-(dates_read - start) <= 0.001) {
      count_size++;
      button++;
      //printf("current value of button: %d\n\n", button);
      //operation(ans, &count_size, line, rorFromLine);
      if (check_stra(line) == 2) {
        sp_t * temp_temp = sp_readline(line);
        base = temp_temp->level_index;
        free(temp_temp);
        //base = sp_readline(line)->level_index;
        //printf("sp check base: %f\n", base);
      }
      else if (check_stra(line) == 1) {
        base = 12;
      }
    }
    else {
      fprintf(stderr,
              "There is something wrong with your input starting date or ending date!\n");
      exit(EXIT_FAILURE);
    }
  }
  //free(line);
  free(line);
  fclose(f);
  return ans;
}

//LOOP
/*
  while ((len = getline(&line, &sz, f)) >= 0) {
    if (read_dates(line) == end) {
      button = 0;
      count_size++;
      ans->n++;
      ans->dates = realloc(ans->dates, count_size * sizeof(*ans->dates));
      ans->ror = realloc(ans->ror, count_size * sizeof(*ans->ror));
      ans->dates[count_size - 1] = read_dates(line);
      if (check_stra(line) == 1) {
        ans->ror[count_size - 1] = rorFromLine(line, );
      }
      else if (check_stra(line) == 0) {
        ans->ror[count_size - 1] = rorFromLine(line);
      }
      break;
    }
    if (button == 1) {
      count_size++;
      ans->n++;
      ans->dates = realloc(ans->dates, count_size * sizeof(*ans->dates));
      ans->ror = realloc(ans->ror, count_size * sizeof(*ans->ror));
      ans->dates[count_size - 1] = read_dates(line);
      if (check_stra(line) == 1) {
        ans->ror[count_size - 1] = rorFromLine(line);
      }
      else if (check_stra(line) == 0) {
        ans->ror[count_size - 1] = rorFromLine(line);
      }
      continue;
    }
    if (read_dates(line) != start) {
      continue;
    }
    if (read_dates(line) == start) {
      button = 1;
      count_size++;
      ans->n++;
      ans->dates = realloc(ans->dates, count_size * sizeof(*ans->dates));
      ans->ror = realloc(ans->ror, count_size * sizeof(*ans->ror));
      ans->dates[count_size - 1] = read_dates(line);
      if (check_stra(line) == 1) {
        ans->ror[count_size - 1] = rorFromLine(line);
      }
      else if (check_stra(line) == 0) {
        ans->ror[count_size - 1] = rorFromLine(line);
      }
    }
  }
}
*/

double cal_power(double num, int power) {
  double ans = 1;
  if (power > 0) {
    for (int i = 1; i <= power; i++) {
      ans *= num;
    }
    return ans;
  }
  else if (power < 0) {
    for (int i = 1; i <= power; i++) {
      ans *= num;
    }
    return 1 / ans;
  }
  else if (power == 0) {
    return ans;
  }
  else {
    exit(EXIT_FAILURE);
  }
}

ret_portfolio_t * getresult(ror_arr_t * array) {
  ret_portfolio_t * ans = malloc(sizeof(*ans));
  int power = 0;
  ans->n = 0;
  ans->cum_invest = NULL;
  ans->dates = NULL;
  for (int i = 0; i < (int)array->n; i++) {
    ans->n++;
    if ((int)(ans->n - 1) % 12 == 0 && (int)(ans->n - 1) != 0) {
      power++;
    }
    ans->dates = realloc(ans->dates, ans->n * sizeof(*ans->dates));
    ans->dates[ans->n - 1] = array->dates[i];
    ans->cum_invest = realloc(ans->cum_invest, ans->n * sizeof(*ans->cum_invest));
    if ((int)ans->n == 1) {
      //ans->cum_invest[ans->n - 1] =
      //0 * (1 + array->ror[i]) + 100 * cal_power(1.25, power);
      ans->cum_invest[ans->n - 1] = 0 * (1 + array->ror[i]) + 100;
      continue;
    }
    ans->cum_invest[ans->n - 1] =
        ans->cum_invest[ans->n - 2] * (1 + array->ror[i]) + 100 * cal_power(1.025, power);
    //printf("this is power: %f\n\n", cal_power(1.25, power));
  }
  return ans;
}

ret_portfolio_t * getfinal(ror_arr_t * arr1, ror_arr_t * arr2) {
  ret_portfolio_t * ans = malloc(sizeof(*ans));
  double diff = 1;
  int power = 0;
  ans->n = 0;
  ans->cum_invest = NULL;
  ans->dates = NULL;
  for (int i = 0; i < (int)arr1->n; i++) {
    ans->n++;
    if ((int)(ans->n - 1) % 12 == 0 && (int)(ans->n - 1) != 0) {
      power++;
      diff -= 0.02;
    }
    ans->dates = realloc(ans->dates, ans->n * sizeof(*ans->dates));
    ans->dates[ans->n - 1] = arr1->dates[i];
    ans->cum_invest = realloc(ans->cum_invest, ans->n * sizeof(*ans->cum_invest));
    if ((int)ans->n == 1) {
      ans->cum_invest[ans->n - 1] =
          0 * (1 + arr1->ror[i]) * diff + 0 * (1 + arr2->ror[i]) * (1 - diff) + 100;
      continue;
    }
    ans->cum_invest[ans->n - 1] =
        ans->cum_invest[ans->n - 2] * (1 + arr1->ror[i]) * diff +
        ans->cum_invest[ans->n - 2] * (1 + arr2->ror[i]) * (1 - diff) +
        100 * cal_power(1.025, power);
  }
  return ans;
}

void free_ror(ror_arr_t * ans) {
  free(ans->dates);
  free(ans->ror);
  free(ans);
}

void free_ret(ret_portfolio_t * ans) {
  free(ans->cum_invest);
  free(ans->dates);
  free(ans);
}

int main(int argc, char ** argv) {
  if (argc != 5) {
    fprintf(stderr, "wrong number of command line arguments\n");
    return EXIT_FAILURE;
  }
  FILE * f1 = fopen(argv[1], "r");
  if (f1 == NULL) {
    //printf("f1 ok\n");
    fprintf(stderr, "cannot open file!\n");
    return EXIT_FAILURE;
  }
  FILE * f2 = fopen(argv[2], "r");
  if (f2 == NULL) {
    //printf("f2 ok\n");
    fprintf(stderr, "cannot open file!\n");
    return EXIT_FAILURE;
  }
  double start = atof(argv[3]);
  //printf("This is the start date: %f\n", start);
  double end = atof(argv[4]);
  //printf("This is the end date: %f\n", end);
  ror_arr_t * ans1 = readFile(f1, start, end, &sp_rorFromline);
  ror_arr_t * ans2 = readFile(f2, start, end, &bond_rorFromline);
  //printf("sp_ror testing: %f\n", ans1->ror[1]);
  //printf("bond_ror testing: %f\n", ans2->ror[1]);
  //printf("the num of sp_n is: %d\n", (int)ans1->n);
  //printf("the num of bond_n is: %d\n", (int)ans2->n);
  //for (int i = 0; i < (int)ans1->n; i++) {
  //printf("%f: %f\n", ans1->dates[i], ans1->ror[i]);
  //}
  //for (int i = 0; i < (int)ans2->n; i++) {
  //printf("%f: %f\n", ans2->dates[i], ans2->ror[i]);
  //}
  ret_portfolio_t * res1 = getresult(ans1);
  ret_portfolio_t * res2 = getresult(ans2);
  ret_portfolio_t * res3 = getfinal(ans1, ans2);
  for (int i = 0; i < (int)res1->n; i++) {
    printf("%.2f,%.2f,%.2f,%.2f\n",
           res1->dates[i],
           res1->cum_invest[i],
           res2->cum_invest[i],
           res3->cum_invest[i]);
  }
  //for (int i = 0; i < (int)res2->n; i++) {
  //printf("%f, %f\n", res2->dates[i], res2->cum_invest[i]);
  //}
  free_ror(ans1);
  free_ror(ans2);
  free_ret(res1);
  free_ret(res2);
  free_ret(res3);
  return EXIT_SUCCESS;
}
