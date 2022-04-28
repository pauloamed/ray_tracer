#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include "../core/light.h"

namespace rt3{

class DirectionalLight : public SamplerLight{
public:
    Vector3f direction;
    real_type mininumFreeDist;

    DirectionalLight(const Color &c, const Vector3f &scl, const Vector3f &lightDirection, real_type dist=10):
        SamplerLight(c, scl), direction(lightDirection.normalize()), mininumFreeDist(dist){}

    
    tuple<Color, unique_ptr<VisibilityTester>, Vector3f> Li(const shared_ptr<Surfel>& hit) override;

};

DirectionalLight* create_directional_light( const ParamSet &ps, Bounds3f worldBox );


}



#endif