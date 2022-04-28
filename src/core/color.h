#ifndef COLOR_H
#define COLOR_H

#include "math_types.h"

namespace rt3{

class Color : public StructuredValues<real_type, 3>{
public:

  Color():StructuredValues<real_type,3>(){}

  Color(const vector<real_type> &v):StructuredValues<real_type, 3>(v){}

  Color clamp(){
    Color newColor(*this);
    for(auto &x : newColor.values){
      x = Clamp<real_type, real_type, real_type>(x, 0, 1);
    }
    return newColor;
  }

  static inline Color interpolate_color(const float t, const Color &a, const Color &b){
    return Color{{
      (real_type) Lerp(t, a.x(), b.x()),
      (real_type) Lerp(t, a.y(), b.y()),
      (real_type) Lerp(t, a.z(), b.z()),
    }};
  }

  static inline Color make_color_from_real(const vector<real_type> &v){
    return Color(v);
  }

  static inline Color make_color_from_int(vector<real_type> v){
    for(auto &x : v) x /= 255;
    return make_color_from_real(v);
  }
};

Color operator+(const Color &x, const Color &y);

Color operator*(const Color &x, const Color &y);

Color operator*(const Color &x, real_type y);

const Color BLACK = Color({0, 0, 0});


class ColorInt : public StructuredValues<int, 3>{
public:

  ColorInt():StructuredValues<int,3>(){}

  ColorInt(const Color &c):ColorInt(){
    for(int i = 0; i < 3; ++i){
      values[i] = c.at(i) * 255;
    }
  }
};


}




#endif