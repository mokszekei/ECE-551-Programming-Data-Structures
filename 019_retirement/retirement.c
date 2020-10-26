#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};

typedef struct _retire_info retire_info;

double balance_calculator(int startAge, double initial, retire_info w_or_r) {
  double balance = initial;
  for (int i = 0; i < w_or_r.months; i++) {
    printf("Age %3d month %2d you have $%.2lf\n", startAge / 12, startAge % 12, balance);
    balance = balance + balance * w_or_r.rate_of_return + w_or_r.contribution;
    startAge++;
  };
  return balance;
}

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  double after_work = balance_calculator(startAge, initial, working);
  balance_calculator(startAge + working.months, after_work, retired);
}

int main(void) {
  retire_info working;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045 / 12;

  retire_info retired;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01 / 12;

  int startAge = 327;
  double initial = 21345;
  retirement(startAge, initial, working, retired);

  return EXIT_SUCCESS;
}
