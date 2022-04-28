#include "blinn_phong.h"

#include "../lights/ambient.h"
#include "../materials/blinn_phong.h"

namespace rt3{

Vector3f computeHalfVector(const Vector3f &viewDir, const Vector3f &lightDir){
    auto h = viewDir + lightDir;
    return h.normalize() * -1;
}

Color BlinnPhongIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor) const{
    return recursiveLi(ray, scene, backgroundColor, 1);
}

Color BlinnPhongIntegrator::recursiveLi(const Ray& ray, const unique_ptr<Scene>& scene, const Color backgroundColor, int currRecurStep) const{
    shared_ptr<ObjSurfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return backgroundColor;
    }else{

        if(isect->wo * isect->n < 0) return BLACK;

        shared_ptr<BlinnPhongMaterial> material = \
            std::dynamic_pointer_cast<BlinnPhongMaterial>(isect->primitive->get_material());

        Color color;
        for(auto &light : scene->lights){
            if(typeid(*light) == typeid(AmbientLight)){
                color = color + light->colorIntensity * material->ambient;
            }else{
                shared_ptr<SamplerLight> samplerLight = std::dynamic_pointer_cast<SamplerLight>(light);

                auto [lightColor, visTester, lightDir] = samplerLight->Li(isect);


                if(visTester->unoccluded(scene)){ // 
                    // difuse
                    {
                        real_type coef = max(real_type(0), isect->n * (lightDir * -1));
                        Color diffuseContrib = material->diffuse * lightColor * coef;
                        
                        color = color + diffuseContrib;
                    }
                    
                    // specular
                    if(material->glossiness){
                        auto h = computeHalfVector(ray.d, lightDir);

                        real_type coef = max(real_type(0), isect->n * h);
                        coef = pow(coef, material->glossiness);
                        Color specularContrib = material->specular * lightColor * coef;
                    
                        color = color + specularContrib;
                    }
                }
            }
        }

        if(currRecurStep < maxRecursionSteps){
            Vector3f newDir = (ray.d + (isect->n * (-2 * (ray.d * isect->n)))).normalize();
            color = color + material->mirror * recursiveLi(Ray(isect->p + newDir * EPS, newDir), scene, backgroundColor, currRecurStep + 1);
        }

        return color;
    }
}


BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet & ps_integrator, unique_ptr<Camera> &&camera){
    return new BlinnPhongIntegrator(
        std::move(camera),
        retrieve(ps_integrator, "depth", int(1))
    );
}


}