#include "bounds.h"

namespace rt3{


bool Bounds3f::box_intersect(const Ray &ray, pair<real_type, real_type> &hits) const{
  hits = {-INF, INF};

  for(int i = 0; i < 3; ++i){
    real_type invDir;

    if(ray.d.at(i) == 0) invDir = INF;
    else invDir = 1.0 / ray.d.at(i);
    
    real_type minT = (minPoint.at(i) - ray.o.at(i)) * invDir;
    real_type maxT = (maxPoint.at(i) - ray.o.at(i)) * invDir;
    if(minT > maxT) swap(minT, maxT);

    hits.first = max(hits.first, minT);
    hits.second = min(hits.second, maxT);
  }
  
  return hits.first < hits.second;
}


bool Bounds3f::intersect_p(const Ray &ray, real_type maxT) const{
  pair<real_type, real_type> hits;
  bool intersects = box_intersect(ray, hits);
  if(!intersects) return false;
  else{
    if(hits.first > 0) return hits.first < maxT;
    else if(hits.second > 0) return hits.second < maxT;
    else return false;
  }
}


Bounds3f Bounds3f::unite(const Bounds3f &a, const Bounds3f &b){
  Bounds3f x(a);
  for(int i = 0; i < 3; ++i){
    x.minPoint[i] = min(a.minPoint.at(i), b.minPoint.at(i));
    x.maxPoint[i] = max(a.maxPoint.at(i), b.maxPoint.at(i));
  }
  return x;
}

Bounds3f Bounds3f::createBox(const vector<Point3f> &p){
	Point3f minPoint(p.front());
	Point3f maxPoint(p.front());

	Point3f epsPoint = Point3f({EPS, EPS, EPS});

	for(int i = 1; i < (int) p.size(); ++i){
		for(int j = 0; j < 3; ++j){
			minPoint[j] = min(minPoint[j], p[i].at(j));
			maxPoint[j] = max(maxPoint[j], p[i].at(j));
		}
	}

	return Bounds3f(minPoint + (epsPoint * -1), maxPoint + epsPoint);
}


vector<Point3f> Bounds3f::getPoints() const{
  vector<Point3f> allPoints;
  vector<Point3f> corners = {minPoint, maxPoint};

  for(int i = 0; i < 8; ++i){
    Point3f currPoint = minPoint;
    for(int j = 0; j < 3; ++j){
      bool isOn = (i & (1 << j));
      currPoint[j] = corners[isOn].at(j);
    }
    allPoints.push_back(currPoint);
  }


  return allPoints;
}


}
