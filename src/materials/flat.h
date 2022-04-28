#ifndef FLAT_MAT_H
#define FLAT_MAT_H

#include "../core/material.h"

namespace rt3{


class FlatMaterial : public Material{
public:
    Color color;
    FlatMaterial(Color c):color(c){}
    Color getColor() const{ return color; }
};


FlatMaterial * create_flat_material( const ParamSet &ps );

}


#endif