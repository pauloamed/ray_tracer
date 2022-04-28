#include "camera.h"

namespace rt3{

pair<real_type, real_type> Camera::get_uv_pos(int i, int j){
    real_type u_pos = sw.width() * (j + 0.5);
    u_pos /= film->width(); u_pos += sw.left;

    real_type v_pos = sw.height() * (i + 0.5);
    v_pos /= film->height(); v_pos += sw.bottom;

    return {u_pos, v_pos};
}

Camera::Camera(unique_ptr<Film> &&_film, Point3f _eye, Point3f _center, Vector3f _up, ScreenWindow _sw):
    film(std::move(_film)), eye(_eye), sw(_sw){
        Vector3f gaze = _center - _eye; 
        
        w = gaze.normalize();
        // std::cout << "w " << w.toString() << std::endl;
        u = _up.cross(w).normalize();
        // std::cout << "u " << u.toString() << std::endl;
        v = u.cross(w).normalize();
        // std::cout << "v " << v.toString() << std::endl;
}

} // namespace rt3