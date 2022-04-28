#ifndef ORTO_CAMERA_H
#define ORTO_CAMERA_H

#include "../core/camera.h"

namespace rt3{

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow sw);
    ~OrthographicCamera();
    
    Ray generate_ray(int x, int y);
};


OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
);

}



#endif