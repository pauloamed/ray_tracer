#ifndef SCENE_H
#define SCENE_H

#include "rt3.h"
#include "background.h"
#include "primitive.h"


namespace rt3{

class Scene{
public:
    unique_ptr<Background> background;
    shared_ptr<Primitive> primitive;
    vector<shared_ptr<Light>> lights;

    Scene(unique_ptr<Background> &&bg, shared_ptr<Primitive> &&prim, vector<shared_ptr<Light>> &&sceneLights):
        background(std::move(bg)), primitive(std::move(prim)), lights(std::move(sceneLights)){}

    ~Scene() = default;

    bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const;
    bool intersect_p(const Ray &r, real_type maxT) const;
};

} // namespace rt3


#endif