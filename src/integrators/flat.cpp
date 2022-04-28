#include "flat.h"
#include "../materials/flat.h"

namespace rt3{

Color FlatIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    
    shared_ptr<ObjSurfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{
        // Some form of determining the incoming radiance at the ray's origin.
        // For this integrator, it might just be:
        // Polymorphism in action.
        shared_ptr<FlatMaterial> fm = std::dynamic_pointer_cast<FlatMaterial>( isect->primitive->get_material() );
        // Assign diffuse color to L.
        return fm->getColor(); // Call a method present only in FlatMaterial.
    }
}

FlatIntegrator* create_flat_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera){
    return new FlatIntegrator(std::move(camera));
}


};