#include "perspective.h"

namespace rt3{

PerspectiveCamera::PerspectiveCamera(
    unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw): 
    Camera(std::move(_film), _eye, _center, _up, _sw){}


PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film
){

    ScreenWindow sw;

    if( ps_camera.count("screen_window")){
        sw = retrieve(ps_camera, "screen_window", ScreenWindow());
    }else if(ps_camera.count("fovy")){
        real_type fovy = Radians(retrieve(ps_camera, "fovy", real_type()));

        real_type aspect = the_film->get_aspect();
        real_type h = fabs(tan(fovy / 2));

        sw = ScreenWindow(
            h * aspect * -1,    // left
            h * aspect,         // right
            h * -1,             // bottom
            h                   // top
        );

        // std::cout << fovy << " " << tan(fovy/2) << " " << h << " " << aspect << std::endl;
        // std::cout << sw.left << " " << sw.right << " " << sw.top << " " << sw.bottom << std::endl;
    }else{
        RT3_ERROR("Can't compute screen window with given parameters.");
    }

    PerspectiveCamera* camera = new PerspectiveCamera(
        std::move(the_film),
        retrieve( ps_look_at, "look_from",  Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at, "look_at",    Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_look_at, "up",         Vector3f({0.0, 0.1, 0.0})),
        sw
    );

    return camera;
}

Ray PerspectiveCamera::generate_ray(int i, int j){
    auto [u_, v_] = get_uv_pos(i, j);
    Vector3f direction = w + (u * u_) + (v * v_);
    return Ray(eye, direction);
}


}