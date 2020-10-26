#include "efficient_frontier.h"

#include <math.h>
#include <stdio.h>

#include <fstream>  // Save to local file.
#include <iomanip>  // for std::setprecision(2)
#include <iostream>
#include <sstream>  // stringstream, getline

#include "Eigen/Dense"  // for matrix manipulation

int main(int argc, char ** argv) {
  if (argc != 4) {
    std::cerr << "Usage: inputFile" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ifstream f1(argv[1]);
  if (!f1) {
    std::cerr << "fail to open " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  std::ifstream f2(argv[2]);
  if (!f2) {
    std::cerr << "fail to open " << argv[2] << std::endl;
    exit(EXIT_FAILURE);
  };
  int shortsell = std::stoi(argv[3]);

  portfolio portfolio;
  try {
    //    portfolio.check_asset(f2, f1);
    portfolio.read_correlation(f2);
    portfolio.read_asset(f1);
    //portfolio.print_asset();
    //portfolio.print_correlation();
    portfolio.calculate_covariance();
    //portfolio.print_covariance();

    /*
    std::pair<double, double> result = portfolio.Lagrange(0.11, shortsell);
    std::cout << "ROR:" << result.first << '\n';
    std::cout << "votality" << result.second << std::endl;
    */

    std::vector<std::pair<double, double> > resultlist;
    for (double i = 1.0; i < 27.0; i++) {
      std::pair<double, double> result = portfolio.Lagrange(i / 100, shortsell);
      resultlist.push_back(result);
    }

    std::cout << "ROR,volatility" << '\n';
    for (int i = 0; i < 26; i++) {
      std::cout << std::setprecision(2) << resultlist[i].first * 100 << "%,"
                << std::setprecision(3) << sqrt(resultlist[i].second) * 100 << "%\n";
    }
  }

  catch (std::exception & e) {
    std::cout << "An exception (" << e.what() << ") occured!\n";
  }
  return (EXIT_SUCCESS);
}
