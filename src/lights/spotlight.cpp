#include "spotlight.h"

namespace rt3{

tuple<Color, unique_ptr<VisibilityTester>, Vector3f> SpotlightLight::Li(const shared_ptr<Surfel>& hit){

    Vector3f direction = hit->p - position;
    real_type angleCos = lightDirection * direction.normalize();
    real_type angle = Degrees(acos(angleCos));

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        position,  // p
        direction.normalize(),
        direction.getNorm()
    );

    
    Color finalColor;
    if(angle > cutoff){
        finalColor = BLACK;
    }else if(angle > falloff){
        finalColor = colorIntensity * pow(1 - ((angle - falloff) / angleInterval), 4);
    }else{
        finalColor = colorIntensity;
    }

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, unique_ptr<VisibilityTester>, Vector3f>{
        finalColor,
        unique_ptr<VisibilityTester>(visTester),
        direction.normalize(),
    };
}

SpotlightLight* create_spotlight_light( const ParamSet &ps ){
    Point3f from = retrieve(ps, "from", Point3f());
    Point3f to = retrieve(ps, "to", Point3f());

    return new SpotlightLight(
        retrieve(ps, "I", Color()),
        retrieve(ps, "scale", Vector3f()),
        from,
        (to - from).normalize(),
        retrieve(ps, "cutoff", real_type()),
        retrieve(ps, "falloff", real_type())
    );
}

}