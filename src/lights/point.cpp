#include "point.h"

namespace rt3{

tuple<Color, unique_ptr<VisibilityTester>, Vector3f> PointLight::Li(const shared_ptr<Surfel>& hit){

    Vector3f direction = hit->p - position;

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        position,  // p
        direction.normalize(),
        direction.getNorm()
    );

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, unique_ptr<VisibilityTester>, Vector3f>{
        colorIntensity,
        unique_ptr<VisibilityTester>(visTester),
        direction.normalize(),
    };
}

PointLight* create_point_light( const ParamSet &ps ){
    return new PointLight(
        retrieve(ps, "I", Color()),
        retrieve(ps, "scale", Vector3f()),
        retrieve(ps, "from", Point3f())
    );
}

}