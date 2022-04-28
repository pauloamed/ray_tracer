#ifndef BLINN_PHONG_INT_H
#define BLINN_PHONG_INT_H

#include "../core/integrator.h"

namespace rt3{

class BlinnPhongIntegrator : public SamplerIntegrator {
private:
    const int maxRecursionSteps;
public:
    ~BlinnPhongIntegrator(){};
    BlinnPhongIntegrator( unique_ptr<Camera> &&_camera, int depth ):
        SamplerIntegrator(std::move(_camera)), maxRecursionSteps(depth){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
    Color recursiveLi(const Ray&, const unique_ptr<Scene>&, const Color, int currRecurStep) const;
};


BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet &, unique_ptr<Camera> &&);


};


#endif 