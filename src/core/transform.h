#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "bounds.h"

namespace rt3{

class Transform {
public:
  Matrix4x4 m, mInv;

  Transform():m(Matrix4x4::getIdentity()), mInv(m){}

  Transform(const Matrix4x4 &matrix) : m(matrix), mInv(matrix.inverse()) { }

  Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) 
    : m(m), mInv(mInv) {
  }

  Transform inverse();
  Transform transpose();

  bool operator==(const Transform &t) const;
  bool operator!=(const Transform &t) const;
  bool IsIdentity() const;

  const Matrix4x4 &GetMatrix() const;
  const Matrix4x4 &GetInverseMatrix() const;

  Point3f apply(const Point3f &p) const;
  Vector3f apply(const Vector3f &v) const;
  Normal3f apply(const Normal3f &n) const;

  Ray apply(const Ray &r) const;
  Bounds3f apply(const Bounds3f &b) const;  
  
  Transform update(const Transform &t2) const;

  static Transform getTranslationMatrix(const Vector3f &v);
  static Transform getRotationMatrix(const Vector3f v, const real_type degrees);
  static Transform getScalingMatrix(const Point3f &p);
  static Transform getIdentityMatrix();
};


const shared_ptr<Transform> IDENTITY = make_shared<Transform>(Transform::getIdentityMatrix());


}

#endif