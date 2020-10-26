#ifndef __RETIREMENT_H__
#define __RETIREMENT_H__

#include <stdlib.h>

struct b_pair_tag {
  double dates;
  double bond_ror;
};
typedef struct b_pair_tag b_pair_t;

struct sp_pair_tag {
  double sp;
  double div;
  double dates;
};
typedef struct sp_pair_tag sp_pair_t;

struct b_ror_arr_tag {
  // WRITE ME
  b_pair_t * b_pair;
  size_t n;
};
typedef struct b_ror_arr_tag b_ror_arr_t;

struct sp_ror_arr_tag {
  // WRITE ME
  sp_pair_t * sp_pair;
  size_t n;
};
typedef struct sp_ror_arr_tag sp_ror_arr_t;

struct summary_tag {
  double dates;
  double bond_ror;
  double sp_ror;
  double sp_percentage;  //decreases this by 2% each year (in January).
  double
      deposit;  ///monthly deposit begins at 100.00 and increases by 2.5% each new year (in January)
};
typedef struct summary_tag summary_t;

struct summary_arr_tag {
  summary_t * summary;
  size_t n;
};
typedef struct summary_arr_tag summary_arr_t;

// struct to hold array of dates and cumulative investments
// (hint: can be used for any investment strategy)
struct ret_portfolio_tag {
  double * dates;
  double * cum_invest;
  size_t n;
};
typedef struct ret_portfolio_tag ret_portfolio_t;

#endif
