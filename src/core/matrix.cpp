#include "matrix.h"
#include "rt3.h"
#include <numeric>
#include <string.h>

namespace rt3{

  Matrix4x4 Matrix4x4::transpose() const{
    vector<vector<real_type>> newM;
    for(size_t i = 0; i < 4; ++i){
      vector<real_type> row;
      for(size_t j = 0; j < 4; ++j){
        row.push_back(m[j][i]);
      }
      newM.push_back(row);
    }
    return Matrix4x4(newM);
  }


  Matrix4x4 Matrix4x4::inverse() const{
    real_type temp[4][4];
    memcpy(temp, m, sizeof temp);

    vector<vector<real_type>> inv(4, vector<real_type>(4));
    for(int i = 0; i < 4; ++i) inv[i][i] = 1;


    int r = 0;
    // processa coluna por coluna
    for(int c = 0; c < 4; ++c){ 
      // acho a linha pivot
      int pivot = r;
      for(int i = r; i < 4; ++i) if(fabs(temp[i][c]) > fabs(temp[pivot][c])) pivot = i;
      if(fabs(temp[pivot][c]) < EPS) continue;

      for(int j = 0; j < 4; ++j) swap(temp[r][j], temp[pivot][j]); // swapo a linha da original
      for(int j = 0; j < 4; ++j) swap(inv[r][j], inv[pivot][j]); // swapo a linha da inversa

      // zerando as linhas
      for(int rr = 0; rr < 4; ++rr){
        if(rr == r) continue;
        real_type x = temp[rr][c] / temp[r][c];
        for(int j = 0; j < 4; ++j) temp[rr][j] -= x * temp[r][j];
        for(int j = 0; j < 4; ++j) inv[rr][j] -= x * inv[r][j];
      }
      r++;
    }

    // norms
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; ++j){
        inv[i][j] /= temp[i][i];
      }
		 }

    
    return Matrix4x4(inv);
  }

  bool Matrix4x4::isIdentity() const{
    int totActive = 0;
    for(auto &x : m) for(int i = 0; i < 4; ++i) totActive += x[i];
    if(totActive != 4) return false;
    for(size_t i = 0; i < 4; ++i){
      if(m[i][i] != 1) return false;
    }
    return true;
  }

  Matrix4x4 Matrix4x4::getIdentity(){
    int n = 4;
    vector<vector<real_type>> m(n, vector<real_type>(n));
    for(int i = 0; i < n; ++i) m[i][i] = 1;
    return Matrix4x4(m);
  }

  Matrix4x4 operator*(const Matrix4x4 &a, const Matrix4x4 &b){
    vector<vector<real_type>> newVals(4, vector<real_type>(4));
    for(int i = 0; i < 4; ++i){
      for(int j = 0; j < 4; ++j){
        for(int k = 0; k < 4; ++k){
          newVals[i][j] += a.m[i][k] * b.m[k][j];
        }
      }
    }

    return Matrix4x4(newVals);
  }

  string Matrix4x4::toString() const{
    std::stringstream ss;
    for(int i = 0; i < 4; ++i){
      for(int j = 0; j < 4; ++j){
        ss << m[i][j] << " ";
      }ss << "\n";
    }
    return ss.str();
  }

}