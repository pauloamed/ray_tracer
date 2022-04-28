#ifndef BLINN_PHONG_MAT_h
#define BLINN_PHONG_MAT_h

#include "../core/material.h"

namespace rt3{


class BlinnPhongMaterial : public Material{
public:
    const Color ambient, diffuse, specular, mirror;
    const int glossiness;

    BlinnPhongMaterial(
        const Color &amb,
        const Color &diffus,
        const Color &spec,
        const Color &mirro,
        int gloss
    ):ambient(amb), diffuse(diffus), specular(spec), mirror(mirro), glossiness(gloss){}
};


BlinnPhongMaterial * create_blinn_phong_material( const ParamSet &ps );

}


#endif