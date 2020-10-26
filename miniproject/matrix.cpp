#include <stdio.h>

#include <fstream>  // Save to local file.
#include <iostream>
#include <sstream>  // stringstream, getline

#include "Eigen/Dense"

using namespace Eigen;

using namespace std;

//a 行
MatrixXd ReadData(istream & data, int a, int b) {
  MatrixXd m_matrix(a, b);
  VectorXd hang(a);
  for (int j = 0; j < a; j++)  //共a 行
  {
    for (int i = 0; i < b; i++)  //共b 列 组成一行
    {
      data >> hang(i);
    }
    m_matrix.row(j) = hang;
  }
  Eigen::VectorXd vOne = Eigen::VectorXd::Constant(b, -1).transpose();
  Eigen::VectorXd vMinusOne = Eigen::VectorXd::Constant(a, 1);
  Eigen::VectorXd vROR = Eigen::VectorXd::Constant(b, -0.01).transpose();
  Eigen::VectorXd vMinusROR = Eigen::VectorXd::Constant(a, 0.01);

  Eigen::MatrixXd matB(2, b);
  matB.row(0) = vOne;
  matB.row(1) = vROR;
  Eigen::MatrixXd matB2(a, 2);
  matB2.col(0) = vMinusOne;
  matB2.col(1) = vMinusROR;

  Eigen::MatrixXd A(a + 2, b + 2);

  A << m_matrix, matB2, matB, Eigen::MatrixXd::Zero(2, 2);
  A(7, 7) = 100;
  return A;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: inputFile" << std::endl;
    exit(EXIT_FAILURE);
  }
  ifstream in(argv[1], ios::in);
  if (!in) {
    return 0;
  }
  MatrixXd m_matrix = ReadData(in, 8, 8);
  cout << m_matrix << endl;

  return 0;
}
