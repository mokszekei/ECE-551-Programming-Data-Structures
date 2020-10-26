
#include "retirement.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function reads each line from a file between the specified dates,
   rorFromLine fcn ptr indicates how ror should be calculated 
   based on type of input data */

b_pair_t rorFromLine_bond(char * str) {
  b_pair_t b_pair;
  char * p = strchr(str, ',');
  if (p == NULL) {
    exit(EXIT_FAILURE);
  }
  p = p + 1;
  b_pair.bond_ror = strtod(p, NULL);
  b_pair.dates = strtod(str, NULL);
  return b_pair;
}

int check_date(char * line, double date) {
  if (strtod(line, NULL) >= date) {
    return 1;
  }
  else {
    return 0;
  }
}
double later_date_calculator(double date) {
  double a = date - (int)date;
  if (a > 0.11) {
    return date + 0.88;
  }
  else {
    return date + 0.01;
  }
}

int check_format(char * line, int c) {
  int len = strlen(line);
  int count = 0;
  for (int i = 0; i < len; i++) {
    if (line[i] == ',') {
      count++;
    }
  }
  if (count == c) {
    return 1;
  }
  else {
    return 0;
  }
}

b_ror_arr_t * readFile_bond(FILE * f,
                            double start,
                            double end,
                            b_pair_t (*rorFromLine)(char *)) {
  // WRITE ME
  if (f == NULL) {
    return NULL;
  }
  char * line = NULL;
  size_t n = 0;

  //read the first line. Be careful! CSV file has header!! actually we should get the second line!
  getline(&line, &n, f);
  getline(&line, &n, f);
  if (check_format(line, 1) == 0) {
    fprintf(stderr, "bond file format is wrong");
    free(line);
    return NULL;
  }

  //start date should be at least one day behind the first data in sp.

  if (start < later_date_calculator(strtod(line, NULL))) {
    fprintf(stderr, "start date is out of bound");
    free(line);
    return NULL;
  }

  b_ror_arr_t * b_ror_arr = malloc(sizeof(*b_ror_arr));
  b_ror_arr->b_pair = NULL;
  b_ror_arr->n = 0;

  if (check_date(line, start) == 1) {
    b_ror_arr->n++;
    b_ror_arr->b_pair =
        realloc(b_ror_arr->b_pair, b_ror_arr->n * sizeof(*b_ror_arr->b_pair));
    b_ror_arr->b_pair[b_ror_arr->n - 1] = rorFromLine(line);
  }

  while (getline(&line, &n, f) >= 0) {
    if (check_date(line, start) == 1) {
      b_ror_arr->n++;
      b_ror_arr->b_pair =
          realloc(b_ror_arr->b_pair, b_ror_arr->n * sizeof(*b_ror_arr->b_pair));
      b_ror_arr->b_pair[b_ror_arr->n - 1] = rorFromLine(line);
    }
    if (check_date(line, end) == 1) {
      break;
    }
  }

  ///check end date

  if (end > strtod(line, NULL)) {
    fprintf(stderr, "end date is out of bound");
    free(line);
    free(b_ror_arr->b_pair);
    free(b_ror_arr);
    return NULL;
  }

  free(line);
  return b_ror_arr;
}

sp_pair_t rorFromLine_sp(char * str) {
  sp_pair_t sp_pair;
  sp_pair.dates = 0;
  sp_pair.div = 0;
  sp_pair.sp = 0;
  char * p1 = strchr(str, ',');
  if (p1 == NULL) {
    exit(EXIT_FAILURE);
  }
  p1++;
  char * p2 = strchr(p1, ',');
  if (p2 == NULL) {
    exit(EXIT_FAILURE);
  }
  sp_pair.div = strtod(p2 + 1, NULL);
  sp_pair.sp = strtod(p1, NULL);
  sp_pair.dates = strtod(str, NULL);
  return sp_pair;
}

double previous_date_calculator(double date) {
  if (date - (int)date == 0) {
    return date - 0.89;
  }
  else {
    return date - 0.01;
  }
}

sp_ror_arr_t * readFile_sp(FILE * f,
                           double start,
                           double end,
                           sp_pair_t (*rorFromLine)(char *)) {
  // WRITE ME
  if (f == NULL) {
    return NULL;
  }
  char * line = NULL;
  size_t n = 0;

  getline(&line, &n, f);
  getline(&line, &n, f);
  if (check_format(line, 2) == 0) {
    fprintf(stderr, "bond file format is wrong");
    free(line);
    return NULL;
  }
  //start date should be at least one day behind the first data in sp.
  if (start < later_date_calculator(strtod(line, NULL))) {
    fprintf(stderr, "start date is out of bound");
    free(line);
    return NULL;
  }

  sp_ror_arr_t * sp_ror_arr = malloc(sizeof(*sp_ror_arr));
  sp_ror_arr->sp_pair = NULL;
  sp_ror_arr->n = 0;
  if (check_date(line, start) == 1) {
    sp_ror_arr->n++;
    sp_ror_arr->sp_pair =
        realloc(sp_ror_arr->sp_pair, sp_ror_arr->n * sizeof(*sp_ror_arr->sp_pair));
    sp_ror_arr->sp_pair[sp_ror_arr->n - 1] = rorFromLine(line);
  }

  while (getline(&line, &n, f) >= 0) {
    if (check_date(line, start) == 1) {
      sp_ror_arr->n++;
      sp_ror_arr->sp_pair =
          realloc(sp_ror_arr->sp_pair, sp_ror_arr->n * sizeof(*sp_ror_arr->sp_pair));
      sp_ror_arr->sp_pair[sp_ror_arr->n - 1] = rorFromLine(line);
    }
    if (check_date(line, end) == 1) {
      break;
    }
  }

  if (end > strtod(line, NULL)) {
    fprintf(stderr, "end date is out of bound");
    free(line);
    free(sp_ror_arr->sp_pair);
    free(sp_ror_arr);
    return NULL;
  }

  free(line);
  return sp_ror_arr;
}

double sp_ror_calculator(double sp, double last_sp, double div) {
  double result = sp / last_sp - 1 + div / 12 / sp;
  return result;
}

double b_ror_calculator(double bond) {
  double result = bond / 1200;
  return result;
}

double cum_calculator(double last_cum, double ror, double deposit) {
  //Cumulative invest(t-1) * [1 + S&P ROR(t)]+ Deposit(t)
  //Cumulative invest(t-1) * [1 + Bond ROR(t)] + Deposit(t)
  double result = last_cum * (1 + ror) + deposit;
  return result;
}

double life_calculator(double last_cum,
                       double sp,
                       double b,
                       double sp_percent,
                       double deposit) {
  //Cumulative invest(t) =
  //Cumulative invest(t-1) * [1 + S&P ROR(t)] * lifecycle allocation(t)+
  //Cumulative invest(t-1) * [1 + Bond ROR(t)] * [1 - lifecycle allocation(t)] + Deposit(t),
  double result =
      last_cum * (1 + sp) * sp_percent + last_cum * (1 + b) * (1 - sp_percent) + deposit;
  return result;
}

summary_arr_t * ror_calculater(sp_ror_arr_t * sp_ror_arr, b_ror_arr_t * b_ror_arr) {
  double start = b_ror_arr->b_pair[0].dates;
  size_t num = b_ror_arr->n - 1;
  summary_arr_t * summary_arr = malloc(sizeof(*summary_arr));
  summary_arr->summary = malloc(num * (sizeof(*summary_arr->summary)));

  for (size_t i = 0; i < num; i++) {
    summary_arr->summary[i].dates = b_ror_arr->b_pair[i + 1].dates;
    summary_arr->summary[i].bond_ror =
        b_ror_calculator(b_ror_arr->b_pair[i + 1].bond_ror);
    summary_arr->summary[i].sp_ror = sp_ror_calculator(sp_ror_arr->sp_pair[i + 1].sp,
                                                       sp_ror_arr->sp_pair[i].sp,
                                                       sp_ror_arr->sp_pair[i + 1].div);
  }
  float c = 0;
  c = start - (int)start;
  if (c > 0.11) {
    for (size_t i = 0; i < num; i++) {
      summary_arr->summary[i].deposit =
          100 * pow(1 + 0.025, ((int)summary_arr->summary[i].dates - (int)start - 1));
      summary_arr->summary[i].sp_percentage =
          1 - 0.02 * ((int)summary_arr->summary[i].dates - (int)start - 1);
    }
  }
  else {
    for (size_t i = 0; i < num; i++) {
      summary_arr->summary[i].deposit =
          100 * pow(1 + 0.025, ((int)summary_arr->summary[i].dates - (int)start));
      summary_arr->summary[i].sp_percentage =
          1 - 0.02 * ((int)summary_arr->summary[i].dates - (int)start);
    }
  }
  summary_arr->n = num;
  return summary_arr;
}

ret_portfolio_t port_initilizer(summary_arr_t * summary_arr) {
  ret_portfolio_t portfolio;
  portfolio.cum_invest = NULL;
  portfolio.dates = NULL;
  size_t n = summary_arr->n + 1;
  portfolio.n = n;
  portfolio.cum_invest = malloc(n * sizeof(*portfolio.cum_invest));
  portfolio.dates = malloc(n * sizeof(*portfolio.dates));
  portfolio.cum_invest[0] = 0;
  portfolio.dates[0] = 0;
  for (size_t i = 1; i < n; i++) {
    portfolio.dates[i] = summary_arr->summary[i - 1].dates;
    portfolio.cum_invest[i] = 0;
  }
  return portfolio;
}

ret_portfolio_t * sp_invest_calculator(summary_arr_t * summary_arr) {
  ret_portfolio_t * portfolio = malloc(sizeof(*portfolio));
  portfolio->cum_invest = NULL;
  portfolio->dates = NULL;

  *portfolio = port_initilizer(summary_arr);
  for (size_t i = 1; i < portfolio->n; i++) {
    portfolio->cum_invest[i] = cum_calculator(portfolio->cum_invest[i - 1],
                                              summary_arr->summary[i - 1].sp_ror,
                                              summary_arr->summary[i - 1].deposit);
  }
  return portfolio;
}
ret_portfolio_t * b_invest_calculator(summary_arr_t * summary_arr) {
  ret_portfolio_t * portfolio = malloc(sizeof(*portfolio));
  portfolio->cum_invest = NULL;
  portfolio->dates = NULL;

  *portfolio = port_initilizer(summary_arr);
  for (size_t i = 1; i < portfolio->n; i++) {
    portfolio->cum_invest[i] = cum_calculator(portfolio->cum_invest[i - 1],
                                              summary_arr->summary[i - 1].bond_ror,
                                              summary_arr->summary[i - 1].deposit);
  }
  return portfolio;
}

ret_portfolio_t * life_invest_calculator(summary_arr_t * summary_arr) {
  ret_portfolio_t * portfolio = malloc(sizeof(*portfolio));
  portfolio->cum_invest = NULL;
  portfolio->dates = NULL;

  *portfolio = port_initilizer(summary_arr);
  for (size_t i = 1; i < portfolio->n; i++) {
    portfolio->cum_invest[i] = life_calculator(portfolio->cum_invest[i - 1],
                                               summary_arr->summary[i - 1].sp_ror,
                                               summary_arr->summary[i - 1].bond_ror,
                                               summary_arr->summary[i - 1].sp_percentage,
                                               summary_arr->summary[i - 1].deposit);
  }
  return portfolio;
}

void free_ror(b_ror_arr_t * b_ror_arr,
              sp_ror_arr_t * sp_ror_arr,
              summary_arr_t * summary_arr) {
  free(b_ror_arr->b_pair);
  free(b_ror_arr);
  free(sp_ror_arr->sp_pair);
  free(sp_ror_arr);
  free(summary_arr->summary);
  free(summary_arr);
}

void free_portfolio(ret_portfolio_t * portfolio) {
  free(portfolio->cum_invest);
  free(portfolio->dates);
  free(portfolio);
}

void write_output(FILE * f,
                  ret_portfolio_t * sp_port,
                  ret_portfolio_t * b_port,
                  ret_portfolio_t * life_port) {
  size_t n = sp_port->n;
  for (size_t i = 1; i < n; i++) {
    fprintf(f,
            "%.2f,%.2f,%.2f,%.2f\n",
            sp_port->dates[i],
            sp_port->cum_invest[i],
            b_port->cum_invest[i],
            life_port->cum_invest[i]);
  }
}

int main(int argc, char ** argv) {
  if (argc != 5) {
    fprintf(stderr, "these programe require four command line argument");
    return EXIT_FAILURE;
  }
  FILE * f_sp = fopen(argv[1], "r");
  if (f_sp == NULL) {
    perror("fopen failed");
    fprintf(stderr, "while trying to read this file");
    return EXIT_FAILURE;
  };

  FILE * f_b = fopen(argv[2], "r");
  if (f_b == NULL) {
    perror("fopen failed");
    fprintf(stderr, "while trying to read this file");
    fclose(f_sp);

    return EXIT_FAILURE;
  };

  FILE * output = fopen("portfolio.csv", "w");
  if (output == NULL) {
    perror("fopen failed");
    fprintf(stderr, "while trying to read this file");
    fclose(f_sp);
    fclose(f_b);

    return EXIT_FAILURE;
  }

  double start = atof(argv[3]);
  double end = atof(argv[4]);
  if (end <= start) {
    fprintf(stderr, "end date should be larger than start date");
    fclose(f_sp);
    fclose(f_b);
    fclose(output);
    return EXIT_FAILURE;
  }

  double test1 = start - (int)start;
  double test2 = end - (int)end;
  if (test1 == 0 || test2 == 0 || test2 > 0.12 || test2 > 0.12) {
    fprintf(stderr, "invalid date input!");
    fclose(f_sp);
    fclose(f_b);
    fclose(output);
    return EXIT_FAILURE;
  }

  sp_ror_arr_t * sp_ror_arr = readFile_sp(f_sp, start, end, rorFromLine_sp);
  b_ror_arr_t * b_ror_arr = readFile_bond(f_b, start, end, rorFromLine_bond);
  if (sp_ror_arr == NULL || b_ror_arr == NULL) {
    fclose(f_sp);
    fclose(f_b);
    fclose(output);
    return EXIT_FAILURE;
  }
  summary_arr_t * summary_arr = ror_calculater(sp_ror_arr, b_ror_arr);
  ret_portfolio_t * sp_port = sp_invest_calculator(summary_arr);
  ret_portfolio_t * b_port = b_invest_calculator(summary_arr);
  ret_portfolio_t * life_port = life_invest_calculator(summary_arr);

  write_output(output, sp_port, b_port, life_port);
  free_ror(b_ror_arr, sp_ror_arr, summary_arr);
  free_portfolio(sp_port);
  free_portfolio(b_port);
  free_portfolio(life_port);
  fclose(f_sp);
  fclose(f_b);
  fclose(output);
  return EXIT_SUCCESS;
}
