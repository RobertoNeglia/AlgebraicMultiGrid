#ifndef _DENSE_MATRIX_H__
#define _DENSE_MATRIX_H__

#include <iostream>
#include <vector>

class MatrixI {
  //---------------------------------------------------------------------------------
  // PRIVATE MEMBERS DECLARATION
  //---------------------------------------------------------------------------------
private:
  unsigned int                  m, n;
  std::vector<std::vector<int>> M;

  //---------------------------------------------------------------------------------
  // PUBLIC MEMBERS DECLARATION
  //---------------------------------------------------------------------------------
public:
  MatrixI() {}
  MatrixI(const unsigned int m, const unsigned int n) : m(m), n(n) {
    M.resize(m, std::vector<int>(n));
  }

  unsigned int
  rows() const {
    return m;
  }

  unsigned int
  cols() const {
    return n;
  }

  void
  insert_coeff(const int val, const unsigned int i, const unsigned int j) {
    M.at(i).at(j) = val;
  }

  // element access in read only (const version)
  const int &
  coeff(const unsigned int i, const unsigned int j) const {
    return M.at(i).at(j);
  }
  // element access in write (returns non-const reference)
  int &
  coeff_ref(const unsigned int i, const unsigned int j) {
    return M.at(i).at(j);
  }
};

#endif