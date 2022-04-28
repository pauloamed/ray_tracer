#ifndef PERS_CAMERA_H
#define PERS_CAMERA_H

#include "../core/camera.h"

namespace rt3{

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow sw);
    ~PerspectiveCamera();
    
    Ray generate_ray(int x, int y);
};

PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
);

}

#endif