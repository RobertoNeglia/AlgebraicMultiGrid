#ifndef _JACOBI_H__
#define _JACOBI_H__

#include <algorithm>
#include <cmath>
#include <vector>

#include "Matrix/sparse_matrix_r.hpp"

class Jacobi {
private:
  SparseMatrix        A;
  std::vector<double> b;
  double              tol;
  unsigned int        max_iter;
  unsigned int        n_iter = 0;

public:
  Jacobi(SparseMatrix A, std::vector<double> b, double tol, unsigned int max_iter) :
    A(A), b(b), tol(tol), max_iter(max_iter) {}

  int
  solve(std::vector<double> &x) {
    std::pair<std::vector<double>, bool> Ax = A.mul(x);
    if (!Ax.second)
      return -1;

    double resid;
    double normb = norm(b);
    if (normb == 0.0)
      normb = 1.0;

    // result of A*x_k
    std::vector<double> b_k = Ax.first;
    // residual at step k: r_k = b - A*x_k = b - b_k;
    std::vector<double> r_k(b_k.size());
    r_k = subvec(b, b_k);

      if ((resid = norm(r_k) / normb) <= tol) {
        tol    = resid;
        n_iter = 0;
        return 0;
    }

    // Preconditioner (Jacobi)
    SparseMatrix        P = get_Jacobi_preconditioner(A);
    std::vector<double> p(b_k.size());

      for (unsigned int i = 0; i < max_iter; i++) {
        // calculate the action of the preconditioner on the residual
        p = P.mul(r_k).first;
        // update the value of the solution
        x = addvec(x, p);
        // update the value of A*x_k
        b_k = A.mul(x).first;
        // calculate the residual
        r_k = subvec(b, b_k);

          if ((resid = norm(r_k) / normb) <= tol) {
            tol    = resid;
            n_iter = i;
            return 1;
        }
      }

    tol    = resid;
    n_iter = max_iter;
    return 2;
  }

  SparseMatrix
  get_Jacobi_preconditioner(const SparseMatrix A) const {
    SparseMatrix D;
    D.initialize(A.cols(), A.rows());

      for (unsigned int i = 0; i < A.cols(); i++) {
        D.insert_coeff(1.0 / A.coeff(i, i).first, i, i);
      }

    return D;
  }

  unsigned int
  get_iter() {
    return n_iter;
  }

  void
  set_max_iter(const unsigned int new_max_iter) {
    max_iter = new_max_iter;
  }

  unsigned int
  get_tol_achieved() {
    return tol;
  }

  void
  setTol(double tol) {
    tol = tol;
  }

  double
  norm(const std::vector<double> a) {
    double n = 0;
      for (unsigned int i = 0; i < a.size(); i++) {
        n += a[i] * a[i];
      }
    return sqrt(n);
  }

  std::vector<double>
  addvec(std::vector<double> a, std::vector<double> y) {
    std::vector<double> result(a.size(), 0.0);
      if (a.size() != y.size()) {
        std::cout << "lele non e' possibile";
        exit(0);
    }
      // result.reserve(a.size());
      for (unsigned int i = 0; i < a.size(); i++) {
        result[i] = a[i] + y[i];
      }
    return result;
  }

  std::vector<double>
  subvec(std::vector<double> a, std::vector<double> y) {
    std::vector<double> result(a.size(), 0.0);
      if (a.size() != y.size()) {
        std::cout << "lele non e' possibile";
        exit(0);
    }

      // result.reserve(a.size());
      for (unsigned int i = 0; i < a.size(); i++) {
        result[i] = a[i] - y[i];
      }
    // cout << result.size() << endl;
    return result;
  }
};

#endif