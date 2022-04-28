#ifndef FLAT_INT_H
#define FLAT_INT_H

#include "../core/integrator.h"

namespace rt3{

class FlatIntegrator : public SamplerIntegrator {
public:
    ~FlatIntegrator(){};


    FlatIntegrator( unique_ptr<Camera> &&_camera ):
        SamplerIntegrator(std::move(_camera)){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
};


FlatIntegrator* create_flat_integrator(const ParamSet &, unique_ptr<Camera> &&);


};


#endif 