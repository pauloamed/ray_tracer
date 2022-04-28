#ifndef AMBIENT_H
#define AMBIENT_H

#include "../core/light.h"

namespace rt3{

class AmbientLight : public Light{
public:
  AmbientLight(const Color &c, const Vector3f &scl):Light(c, scl){};
  virtual void preprocess( const Scene & ){};
};

AmbientLight* create_ambient_light( const ParamSet &ps );

}



#endif