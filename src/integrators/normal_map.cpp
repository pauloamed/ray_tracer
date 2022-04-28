#include "normal_map.h"

namespace rt3{


Color NormalIntegrator::getColorFromNormal(const Vector3f &n) const{
    Vector3f inRange = (n + Vector3f({1, 1, 1})) * real_type(0.5);
    return Color({
        inRange.at(0),
        inRange.at(1),
        inRange.at(2),
    }).clamp();
}

Color NormalIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    shared_ptr<ObjSurfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        return getColorFromNormal(isect->n);      
    }
}

NormalIntegrator* create_normal_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new NormalIntegrator(std::move(camera));
}

};