#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.h"
#include "camera.h"
#include "surfel.h"


namespace  rt3 {

class Integrator {
public:
    virtual ~Integrator(){};
    virtual void render( const unique_ptr<Scene>& ) = 0;
};


class SamplerIntegrator : public Integrator {
//=== Public interface
public:
    virtual ~SamplerIntegrator(){};
        // Default init    

    SamplerIntegrator( unique_ptr<Camera> &&_camera ){
        camera = std::move(_camera);
    }

    virtual Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const = 0;
    virtual void render( const unique_ptr<Scene>& );
    // virtual void preprocess( const unique_ptr<Scene>& );
    
protected:
    std::unique_ptr<Camera> camera;
    int getColorFromCoord(real_type x) const;
};



} // namespace rt3



#endif // INTEGRATOR_H