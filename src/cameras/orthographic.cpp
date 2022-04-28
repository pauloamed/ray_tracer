#include "orthographic.h"

namespace rt3{
Ray OrthographicCamera::generate_ray(int i, int j){
    auto [u_, v_] = get_uv_pos(i, j);
    Point3f origin = eye + (u * u_) + (v * v_);
    return Ray(origin, w);
}


OrthographicCamera::OrthographicCamera(
    unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw): 
    Camera(std::move(_film), _eye, _center, _up, _sw){}




OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
){

    OrthographicCamera* camera = new OrthographicCamera(
        std::move(the_film),
        retrieve( ps_look_at,   "look_from",        Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at,   "look_at",          Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at,   "up",               Vector3f({0.0, 0.1, 0.0})),
        retrieve( ps_camera,    "screen_window",    ScreenWindow())
    );

    return camera;
}


}