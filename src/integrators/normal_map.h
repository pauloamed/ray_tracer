#ifndef NORMAL_MAP_H
#define NORMAL_MAP_H

#include "../core/integrator.h"

namespace rt3{

class NormalIntegrator : public SamplerIntegrator {
private:
    Color getColorFromNormal(const Vector3f &n) const;
public:
    ~NormalIntegrator(){};


    NormalIntegrator( unique_ptr<Camera> &&_camera ):
        SamplerIntegrator(std::move(_camera)){}

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
};

NormalIntegrator* create_normal_integrator(const ParamSet &, unique_ptr<Camera> &&);

}

#endif