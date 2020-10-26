
#include <stdio.h>

#include <cstdlib>
#include <exception>
#include <fstream>  // Save to local file.
#include <iostream>
#include <vector>

#include "Eigen/Dense"
#include "Eigen/LU"

class invalid_cor_input : public std::exception {
  const char * what() const throw() { return "invalid correlation input"; }
};
class invalid_a_input : public std::exception {
  const char * what() const throw() { return "invalid asset input"; }
};

class wrong_asset : public std::exception {
  const char * what() const throw() {
    return "asset file and correlation are not corresponding to each other";
  }
};

//check whether weight vector contain negetive element(concergence criteria)
std::vector<int> checkPositive(std::vector<double> w) {
  std::vector<int> index;
  int i = 0;
  std::vector<double>::iterator it = w.begin();
  while (it != w.end()) {
    if (*it < 0) {
      index.push_back(i);
    }
    i++;
    ++it;
  }
  return index;
}
// Update weight vector when no shortsell allowed
std::vector<double> calculateWeight(std::vector<std::vector<double> > A,
                                    std::vector<double> v) {
  std::size_t n = v.size();

  Eigen::MatrixXd newA(n, n);
  for (std::size_t i = 0; i < n; i++) {
    for (std::size_t j = 0; j < n; j++) {
      newA(i, j) = A[i][j];
    }
  }

  Eigen::VectorXd newv(n);
  for (std::size_t i = 0; i < n; i++) {
    newv(i) = v[i];
  }
  Eigen::VectorXd ans(n);

  ans = newA.inverse() * newv;

  std::vector<double> weight;
  for (std::size_t i = 0; i < n; i++) {
    weight.push_back(ans(i));
  }

  return weight;
}
//The algorithm finding the final variance when no shortsell allowed
// Algorithm description:
// 1.Find the asset with weight<0 and delete the corresponding asset in covariance matrix ,weight vector
// and the vector used to multiply the assembly matrix in KKT system.( Size of them will be redueced)
// 2. Solve the new KKT system.
// 3. Check whether weight vector still contain negetive element. If so repeat step 1-3.If not return variance.
std::pair<double, std::vector<double> > getVarianceWithoutShortsell(
    std::vector<std::vector<double> > A,
    std::vector<double> w,
    std::vector<double> v) {
  double variance = 0;
  std::vector<int> index = checkPositive(w);
  while (index.size() != 0) {
    for (std::size_t i = 0; i < index.size(); i++) {
      v.erase(v.begin() + index[i] - i);
      A.erase(A.begin() + index[i] - i);
      for (std::size_t j = 0; j < A.size(); j++) {
        A[j].erase(A[j].begin() + index[i] - i);
      }
    }
    w.erase(w.begin(), w.end());
    w = calculateWeight(A, v);
    index = checkPositive(w);
  }

  for (std::size_t i = 0; i < w.size(); i++) {
    for (std::size_t j = 0; j < w.size(); j++) {
      variance += A[i][j] * w[i] * w[j];
    }
  }

  std::pair<double, std::vector<double> > result;
  result.first = variance;
  result.second = w;
  return result;
}

//The main class.
class portfolio {
 private:
  std::vector<std::pair<double, double> > asset;
  std::vector<std::vector<double> > correlation;
  std::vector<std::vector<double> > covariance;
  int nA;

 public:
  // Read correlation file to assemble correlation matrix
  void read_correlation(std::ifstream & f) {
    std::string line;
    std::getline(f, line);
    int row_num = 0;
    while (std::getline(f, line)) {
      std::vector<double> temp;
      int col_num = 0;
      std::size_t p1 = 0;
      std::size_t p2 = 0;
      while (line.find(',', p2) != std::string::npos) {
        p1 = line.find(',', p2);
        p2 = line.find('%', p1);
        if (p2 == std::string::npos) {
          throw invalid_cor_input();
        }
        std::string key = line.substr(p1 + 1, p2 - p1 - 1);
        if (!isdigit(key[0]) && key[0] != '-') {
          throw invalid_cor_input();
        }
        temp.push_back(std::stod(key) / 100);
        col_num++;
      }
      if (row_num == 0) {
        nA = col_num;
      }
      else {
        if (col_num != nA) {
          throw invalid_cor_input();
        }
      }
      correlation.push_back(temp);
      row_num++;
    }
    if (nA != row_num) {
      throw wrong_asset();
    }
  }
  //print correlation
  void print_correlation(void) {
    int n = correlation.size();
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << correlation[i][j] << ' ';
        if (j == n) {
          std::cout << '\n';
        }
      }
    }
  }
  // Read assets file
  void read_asset(std::ifstream & f) {
    std::string line;
    std::getline(f, line);
    int row_num = 0;
    while (std::getline(f, line)) {
      std::size_t p1 = 0;
      std::size_t p2 = 0;
      std::pair<double, double> temp;
      p1 = line.find(',');
      p2 = line.find('%');
      if (p2 == std::string::npos) {
        throw invalid_a_input();
      }
      std::string key = line.substr(p1 + 1, p2 - p1 - 1);
      if (!isdigit(key[0]) && key[0] != '-') {
        throw invalid_a_input();
      }
      temp.first = std::stod(key) / 100;
      p1 = line.find(',', p2);
      p2 = line.find('%', p1);
      if (p2 == std::string::npos) {
        throw invalid_a_input();
      }
      std::string key2 = line.substr(p1 + 1, p2 - p1 - 1);
      if (!isdigit(key2[0]) && key2[0] != '-') {
        throw invalid_a_input();
      }
      temp.second = std::stod(key2) / 100;
      asset.push_back(temp);
      row_num++;
    }
    if (nA != row_num) {
      throw wrong_asset();
    }
  }
  void print_asset(void) {
    int n = asset.size();
    for (int i = 0; i < n; i++) {
      std::cout << asset[i].first << ' ' << asset[i].second << '\n';
    }
  }

  void calculate_covariance(void) {
    //cov(i,j) = correl(i,j)*std(i)*std(j)
    for (int i = 0; i < nA; i++) {
      std::vector<double> temp;
      for (int j = 0; j < nA; j++) {
        temp.push_back(correlation[i][j] * asset[i].first * asset[j].first);
      }
      covariance.push_back(temp);
    }
  }

  void print_covariance(void) {
    int n = covariance.size();
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << correlation[i][j] << ' ';
        if (j == n) {
          std::cout << '\n';
        }
      }
    }
  }
  // Check whether asset order in both file are the same, Otherwise we will get wrong answer.
  void check_asset(std::ifstream & cor, std::ifstream & ass) {
    std::string line1;
    std::string line2;
    std::getline(cor, line1);
    std::getline(ass, line2);
    while (std::getline(cor, line1)) {
      std::getline(ass, line2);
      std::string key1 = line1.substr(0, line1.find(','));
      std::string key2 = line2.substr(0, line2.find(','));
      if (key1.compare(key2) != 0) {
        throw wrong_asset();
      }
    }
  }

  ///Quadratic programming with desired E[ROR]; and whether short sells are permitted.0=no shortsell, 1=short sell
  std::pair<double, double> Lagrange(double ROR, int shortSell) {
    int n = nA;
    double variance = 0;
    ///KKT system assembly for weight computation
    Eigen::MatrixXd cov_m(n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        cov_m(i, j) = covariance[i][j];
      }
    }
    Eigen::VectorXd vOne = Eigen::VectorXd::Constant(n, 1).transpose();
    Eigen::VectorXd vMinusOne = Eigen::VectorXd::Constant(n, -1);
    Eigen::VectorXd vROR(n);
    Eigen::VectorXd vMinusROR(n);
    for (int i = 0; i < n; i++) {
      vMinusROR[i] = -asset[i].second;
    }
    vROR = -vMinusROR;
    Eigen::MatrixXd matB(2, n);
    matB.row(0) = vOne;
    matB.row(1) = vROR.transpose();
    Eigen::MatrixXd matB2(n, 2);
    matB2.col(0) = vMinusOne;
    matB2.col(1) = vMinusROR;
    Eigen::MatrixXd A(n + 2, n + 2);
    A << cov_m, matB2, matB, Eigen::MatrixXd::Zero(2, 2);
    Eigen::VectorXd v1 = Eigen::VectorXd::Constant(n, 0);
    Eigen::VectorXd v(n + 2);
    v << v1, 1, ROR;

    // Solve the KKK system
    //DO we need to check whether A is invertible?
    Eigen::VectorXd ans(n + 2);
    ans = A.inverse() * v;

    ///Just to check whether KKT system assembly is correct
    /*
    std::cout << "Matrix A is:\n" << A << std::endl;
    std::cout << "Matrix A inverse is:\n" << A.inverse() << std::endl;
    std::cout << "Vector is:\n" << v << std::endl;
    std::cout << "Answer is:\n" << ans << std::endl;
    */
    Eigen::VectorXd w(n);
    for (int i = 0; i < n; i++) {
      w(i) = ans(i);
    }
    //std::cout << "weight is:\n" << w << std::endl;
    std::vector<double> weight;
    for (int i = 0; i < n; i++) {
      weight.push_back(w(i));
    }

    std::pair<double, double> result;
    result.first = ROR;

    if (shortSell == 1) {
      //var = sum_i sum_j w(i)*w(j)*cov(i,j)

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          variance += covariance[i][j] * w(i) * w(j);
        }
      }

      result.second = variance;
    }

    else if (shortSell == 0) {
      //To better handel the matrix, put it into vector<vector>;
      std::vector<std::vector<double> > matA;
      for (int i = 0; i < n + 2; i++) {
        std::vector<double> temp;
        for (int j = 0; j < n + 2; j++) {
          temp.push_back(A(i, j));
        }
        matA.push_back(temp);
      }
      std::vector<double> vector;
      for (int i = 0; i < n + 2; i++) {
        vector.push_back(v(i));
      }
      std::pair<double, std::vector<double> > answer;
      answer = getVarianceWithoutShortsell(matA, weight, vector);
      result.second = answer.first;
      weight = answer.second;
    }
    return result;
  }

  ~portfolio() { std::cout << "Succeddfully destruct the class" << std::endl; };
};
