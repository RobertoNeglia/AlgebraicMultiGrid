#include <chrono>
#include <functional>

#include "LinearAlgebra/IterativeSolvers/algebraic_multigrid.hpp"
#include "LinearAlgebra/IterativeSolvers/geometric_multigrid.hpp"

void
generate_discretized_matrix(SparseMatrix &A, int m, int n) {
  int mn = m * n;
    for (int i = 0; i < mn; i++) {
      A.insert_coeff(4, i, i);
      if (i > 0 && i % m != 0)
        A.insert_coeff(-1, i, i - 1);
      if (i > m - 1)
        A.insert_coeff(-1, i, i - m);
      if (i < mn - 1 && (i % m) != (m - 1))
        A.insert_coeff(-1, i, i + 1);
      if (i < mn - m)
        A.insert_coeff(-1, i, i + m);
    }
}

auto
timeit(const std::function<void()> &f) {
  using namespace std::chrono;
  const auto start = high_resolution_clock::now();
  f();
  const auto end = high_resolution_clock::now();
  return duration_cast<milliseconds>(end - start).count();
}

int
main(int argc, char **argv) {
  unsigned int N = 10;
    if (argc >= 2) {
      N = std::atoi(argv[1]);
  }

  unsigned int n = (N - 1) * (N - 1);

  SparseMatrix A;
  A.initialize(n, n);

  generate_discretized_matrix(A, N - 1, N - 1);

  double exact_sol = 1.0;
  // exact solution of the linear system
  Vector<double> x(A.cols(), exact_sol);
  // fill_random(x);
  // system rhs
  Vector<double> b = A.mul(x);

  // GMG initial guess
  double         initial_guess = 0.0;
  Vector<double> jac_guess(A.cols(), initial_guess);

  const double       tol      = 1.e-8;
  const unsigned int max_iter = 30000;

  Jacobi jac(A, b, tol, max_iter);

  auto dt = timeit([&]() { jac.solve(jac_guess); });

  std::cout << "NAIVE JAC: time elapsed " << dt << " [ms]" << std::endl;
  std::cout << "NAIVE JAC: n_iter " << jac.get_iter() << std::endl;
  std::cout << "NAIVE JAC: tol_achived " << jac.get_tol_achieved() << std::endl << std::endl;

#ifdef _OPENMP
  std::cout << "ER SAMBUCONE MOLINARI CURATIVO" << std::endl;
#endif

  return 0;
}