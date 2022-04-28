#ifndef SURFEL_H
#define SURFEL_H

#include "rt3.h"

namespace rt3{

struct Surfel{
    Point3f p;        //!< Contact point.
    Vector3f wo;      //!< Outgoing direction of light, which is -ray.
    
    // se o ray veio de um obj ou camera, t tem eh o coef q multiplica o raio saindo do obj ou camera ate surfel
    // se o ray veio da luz, t eh o coef do raio saindo da luz (mas note q o surfel eh a luz, nao a intersect)
    real_type t; 
    
    Surfel(const Point3f& _p, const Vector3f& _wo, real_type _t):p(_p), wo(_wo.normalize()), t(_t){}
};

struct ObjSurfel : public Surfel{
    Normal3f n;       //!< The surface normal.
    shared_ptr<const GeometricPrimitive> primitive=nullptr; //!< Pointer to the primitive.
    

    ObjSurfel(const Point3f& _p, const Normal3f& _n, const Vector3f& _wo, real_type _t)
        : Surfel(_p, _wo, _t), n{_n}{ 
            n = n.normalize(); 
        };

    void setPrimitive(const shared_ptr<const GeometricPrimitive> &prim){
        primitive = prim;
    }
};

} // namespace rt3






#endif