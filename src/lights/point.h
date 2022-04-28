#ifndef POINT_H
#define POINT_H

#include "../core/light.h"

namespace rt3{

class PointLight : public SamplerLight{
public:
    Point3f position;

    PointLight(const Color &c, const Vector3f &scl, const Point3f &pos):
        SamplerLight(c, scl), position(pos){}

    
    tuple<Color, unique_ptr<VisibilityTester>, Vector3f> Li(const shared_ptr<Surfel>& hit) override;

};

PointLight* create_point_light( const ParamSet &ps );


}



#endif