#include "scene.h"

namespace rt3{
    bool Scene::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
        return primitive->intersect(r, isect);
    }

    bool Scene::intersect_p(const Ray &r, real_type maxT) const{
        return primitive->intersect_p(r, maxT);
    }

}