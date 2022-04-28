#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "film.h"
#include "screen_window.h"

namespace rt3{

class Camera{
public:
    unique_ptr<Film> film;
    Point3f eye; // position of the camera
    Vector3f w, u, v;
    ScreenWindow sw;

    Camera(unique_ptr<Film> &&film, Point3f eye, Point3f center, Vector3f up, ScreenWindow sw);
    
    pair<real_type, real_type> get_uv_pos(int i, int j);

    virtual Ray generate_ray(int x, int y) = 0;
    // virtual ~Camera() = 0;
};


} // namespace rt3

#endif