#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#include "../core/integrator.h"

namespace rt3{

class DepthMapIntegrator : public SamplerIntegrator {
private:
    real_type zmin, zmax, z_range;
    Color near_color, far_color;

    real_type scene_tmin, scene_tmax, t_range;
    void preprocess(const unique_ptr<Scene>&);

    real_type normalizeZ(real_type x) const;
    real_type normalizeT(real_type x) const;

public:
    ~DepthMapIntegrator(){};

    


    DepthMapIntegrator( unique_ptr<Camera> &&_camera, real_type z_min, real_type z_max, Color n_color, Color f_color ):
        SamplerIntegrator(std::move(_camera)), zmin(z_min), zmax(z_max), near_color(n_color), far_color(f_color){

        scene_tmin = INFINITY;
        scene_tmax = 0;
        z_range = z_max - z_min;
    }

    Color Li(const Ray&, const unique_ptr<Scene>&, const Color) const override;
    void render( const unique_ptr<Scene>& ) override;
};




DepthMapIntegrator* create_depth_map_integrator(const ParamSet &, unique_ptr<Camera> &&);

};


#endif