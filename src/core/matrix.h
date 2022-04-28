#ifndef MATRIX_H
#define MATRIX_H

#include "math_base.h"

namespace rt3{

struct Matrix4x4{
  real_type m[4][4];


  Matrix4x4(){}

  Matrix4x4(const vector<vector<real_type>> &matrix){
    for(size_t i = 0; i < matrix.size(); ++i){
      for(size_t j = 0; j < matrix[i].size(); ++j){
        m[i][j] = matrix[i][j];
      }
    }
  }

  Matrix4x4 transpose() const;
  Matrix4x4 inverse() const;

  bool isIdentity() const;

  friend bool operator==(const Matrix4x4 &a, const Matrix4x4 &b){ return a.m == b.m; }

  friend bool operator!=(const Matrix4x4 &a, const Matrix4x4 &b){ return a.m != b.m; }

  friend Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b);

  static Matrix4x4 getIdentity();

  string toString() const;

  };

}


#endif 