#include "color.h"

namespace rt3{
Color operator+(const Color &x, const Color &y){
    Color v;
    for(int i = 0; i < 3; ++i){
        v[i] = x.at(i) + y.at(i);
    }
    return v.clamp();
}

Color operator*(const Color &x, const Color &y){
    Color v;
    for(int i = 0; i < 3; ++i){
        v[i] = x.at(i) * y.at(i);
    }
    return v.clamp();
}

Color operator*(const Color &x, real_type y){
    Color v;
    for(int i = 0; i < 3; ++i){
        v[i] = x.at(i) * y;
    }
    return v.clamp();
}

};
