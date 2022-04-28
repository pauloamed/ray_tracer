#ifndef BOUNDS_H
#define BOUNDS_H

#include "math_base.h"
#include "rt3.h"

namespace rt3{


class Bounds3f{
// private:
  // This is an axis-aligned 3d box. Therefore, for each axis, there are only two possible values.
  // (a box has 8 vertexes = 2^3 (for each of the 3 axis, there are 2 possible values))

  // minPoint will keep the minimum values for each axis
  // maxPoint will keep the maximum values for each axis
  // it is a non-degenerated box. Thus, values across the same axis are distincts


public:
  Point3f minPoint, maxPoint; 
  Bounds3f():minPoint(Point3f({INF, INF, INF})), maxPoint(Point3f({-INF, -INF, -INF})){}
  Bounds3f(Point3f minP, Point3f maxP):minPoint(minP), maxPoint(maxP){}

  bool box_intersect(const Ray &ray, pair<real_type, real_type> &hits) const;
  bool intersect_p(const Ray &ray, real_type maxT) const;
  vector<Point3f> getPoints() const;

  static Bounds3f createBox(const vector<Point3f> &p);
  static Bounds3f unite(const Bounds3f &a, const Bounds3f &b);
};



}




#endif