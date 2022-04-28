#include "transform.h"

namespace rt3{

Transform Transform::inverse() {
    return Transform(mInv, m);
}

Transform Transform::transpose() {
    return Transform(m.transpose(), mInv.transpose());
}

bool Transform::operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
}

bool Transform::operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
}

bool Transform::IsIdentity() const { return m.isIdentity(); }

const Matrix4x4 &Transform::GetMatrix() const { return m; }

const Matrix4x4 &Transform::GetInverseMatrix() const { return mInv; }

Point3f Transform::apply(const Point3f &p) const{
  Point3f ret = {{
    m.m[0][0] * p.x() + m.m[0][1] * p.y() + m.m[0][2] * p.z() + m.m[0][3],
    m.m[1][0] * p.x() + m.m[1][1] * p.y() + m.m[1][2] * p.z() + m.m[1][3],
    m.m[2][0] * p.x() + m.m[2][1] * p.y() + m.m[2][2] * p.z() + m.m[2][3],
  }};
  return ret;
}

Vector3f Transform::apply(const Vector3f &v) const{
  Vector3f ret = {{
    m.m[0][0] * v.x() + m.m[0][1] * v.y() + m.m[0][2] * v.z(),
    m.m[1][0] * v.x() + m.m[1][1] * v.y() + m.m[1][2] * v.z(),
    m.m[2][0] * v.x() + m.m[2][1] * v.y() + m.m[2][2] * v.z(),
  }};
  return ret;
}

Normal3f Transform::apply(const Normal3f &n) const{
  Normal3f ret = {{
    mInv.m[0][0] * n.x() + mInv.m[1][0] * n.y() + mInv.m[2][0] * n.z(),
    mInv.m[0][1] * n.x() + mInv.m[1][1] * n.y() + mInv.m[2][1] * n.z(),
    mInv.m[0][2] * n.x() + mInv.m[1][2] * n.y() + mInv.m[2][2] * n.z(),
  }};
  return ret;
}

Ray Transform::apply(const Ray &r) const{
  return Ray(apply(r.o), apply(r.d));
}

Bounds3f Transform::apply(const Bounds3f &b) const{
  vector<Point3f> allPoints = b.getPoints();
  for(auto &p : allPoints) p = apply(p);
  return Bounds3f::createBox(allPoints);
}

Transform Transform::update(const Transform &t2) const{
  return Transform(m * t2.m);
}


Transform Transform::getTranslationMatrix(const Vector3f &v){
  vector<vector<real_type>> m = {
    {1, 0, 0, v.at(0)},
    {0, 1, 0, v.at(1)},
    {0, 0, 1, v.at(2)},
    {0, 0, 0, 1}
  };  
  vector<vector<real_type>> inv_m = {
    {1, 0, 0, -v.at(0)},
    {0, 1, 0, -v.at(1)},
    {0, 0, 1, -v.at(2)},
    {0, 0, 0, 1}
  };
  return Transform(Matrix4x4(m), Matrix4x4(inv_m));
}

Transform Transform::getRotationMatrix(Vector3f v, const real_type degrees){
  v = v.normalize();
  real_type rotCos = cos(Radians(degrees));
  real_type rotSin = sin(Radians(degrees));

  real_type xx = v.x() * v.x();
  real_type xy = v.x() * v.y();
  real_type xz = v.x() * v.z();
  real_type yy = v.y() * v.y();
  real_type yz = v.y() * v.z();
  real_type zz = v.z() * v.z();
  real_type compC = 1 - rotCos;
  real_type xs = v.x() * rotSin;
  real_type ys = v.y() * rotSin;
  real_type zs = v.z() * rotSin;

  vector<vector<real_type>> m = {
    { xx + (1 - xx) * rotCos, xy * compC - zs, xz * compC + ys, 0},
    { xy * compC + zs, yy + (1 - yy) * rotCos, yz * compC - xs, 0},
    { xz * compC - ys, yz * compC + xs, zz + (1 - zz) * rotCos, 0},
    { 0, 0, 0, 1},
  };

  Matrix4x4 matrix(m);

  return Transform(matrix);
}

Transform Transform::getScalingMatrix(const Point3f &p){
  vector<vector<real_type>> m = {
    {p.at(0), 0, 0, 0},
    {0, p.at(1), 0, 0},
    {0, 0, p.at(2), 0},
    {0, 0, 0, 1}
  };
  vector<vector<real_type>> inv_m = {
    {1/p.at(0), 0, 0, 0},
    {0, 1/p.at(1), 0, 0},
    {0, 0, 1/p.at(2), 0},
    {0, 0, 0, 1}
  };
  
  return Transform(Matrix4x4(m), Matrix4x4(inv_m));
}


Transform Transform::getIdentityMatrix(){
  return Transform();
}



}