#ifndef SHAPE_H
#define SHAPE_H

#include "math_base.h"
#include "paramset.h"
#include "surfel.h"
#include "bounds.h"

namespace rt3{

class Shape{
public:
    bool flip_normals;

    ~Shape(){}

    virtual Bounds3f computeBounds() const = 0;

    virtual bool intersect_p(const Ray &r, real_type maxT) const = 0;
    virtual bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const = 0;
};

} // namespace rt3










#endif