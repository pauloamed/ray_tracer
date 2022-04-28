#include "blinn_phong.h"

namespace rt3{
BlinnPhongMaterial * create_blinn_phong_material( const ParamSet &ps ){
    return new BlinnPhongMaterial(
        retrieve(ps, "ambient", Color()),
        retrieve(ps, "diffuse", Color()),
        retrieve(ps, "specular", Color()),
        retrieve(ps, "mirror", Color()),
        retrieve(ps, "glossiness", int())
    );
}
}