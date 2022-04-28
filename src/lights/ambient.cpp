#include "ambient.h"

namespace rt3{

AmbientLight* create_ambient_light( const ParamSet &ps ){
    return new AmbientLight(
        retrieve(ps, "L", Color()),
        retrieve(ps, "scale", Vector3f())
    );
}

}