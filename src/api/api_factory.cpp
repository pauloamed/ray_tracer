#include "api.h"
#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"

#include "../integrators/flat.h"
#include "../integrators/depth_map.h"
#include "../integrators/normal_map.h"
#include "../integrators/blinn_phong.h"

#include "../materials/flat.h"
#include "../materials/blinn_phong.h"

#include "../lights/directional.h"
#include "../lights/ambient.h"
#include "../lights/spotlight.h"
#include "../lights/point.h"

#include "../shapes/sphere.h"
#include "../shapes/triangle.h"

namespace rt3 {

Film * API::make_film( const ParamSet &ps )
{
    std::cout << ">>> Inside API::make_film()\n";
    Film *film{ nullptr };
    film = create_film( ps );

    // Return the newly created film.
    return film;
}


Background * API::make_background( const ParamSet& ps )
{
    std::cout << ">>> Inside API::make_background()\n";
    Background *bkg{ nullptr };

    bg_type_t type = retrieve(ps, "type", bg_type_t::colors);

    if(type == bg_type_t::colors){
        bkg = create_color_background( ps );
    }else{
        RT3_ERROR("Unknown background type!");
    }
    
    // Return the newly created background.
    return bkg;
}


Camera * API::make_camera( const ParamSet& ps_camera,
    const ParamSet& ps_look_at, unique_ptr<Film>&& the_film )
{
    std::cout << ">>> Inside API::make_camera()\n";

    camera_type_t type = retrieve(ps_camera, "type", camera_type_t::orthographic);

    Camera *camera = nullptr;
    if(type == camera_type_t::orthographic){
        camera = create_orthographic_camera(ps_camera, ps_look_at, std::move(the_film));
    }else if(type == camera_type_t::perspective){
        camera = create_perspective_camera(ps_camera, ps_look_at, std::move(the_film));
    }

    return camera;
}


Integrator * API::make_integrator( const ParamSet &ps_integrator,
        unique_ptr<Camera> &&camera )
{
    std::cout << ">>> Inside API::make_integrator()\n";
    Integrator* integ = nullptr;

    integrator_type_t type = retrieve(ps_integrator, "type", integrator_type_t::flat);
    if(type == integrator_type_t::flat){
        integ = create_flat_integrator(ps_integrator, std::move(camera));
    }else if(type == integrator_type_t::depth_map){
        integ = create_depth_map_integrator(ps_integrator, std::move(camera));
    }else if(type == integrator_type_t::normal_map){
        integ = create_normal_integrator(ps_integrator, std::move(camera));
    }else if(type == integrator_type_t::blinn_phong){
        integ = create_blinn_phong_integrator(ps_integrator, std::move(camera));
    }else{
        RT3_ERROR("Integrator type unknown.");
    }
    
    // Return the newly created integrator
    return integ;
}


Light * API::make_light( const ParamSet &ps_light, Bounds3f worldBox )
{
    std::cout << ">>> Inside API::make_light()\n";
    Light* light = nullptr;

    light_type_t type = retrieve(ps_light, "type", light_type_t::ambient);
    if(type == light_type_t::ambient){
        light = create_ambient_light(ps_light);
    }else if(type == light_type_t::directional){
        light = create_directional_light(ps_light, worldBox);
    }else if(type == light_type_t::point){
        light = create_point_light(ps_light);
    }else if(type == light_type_t::spot){
        light = create_spotlight_light(ps_light);
    }else{
        RT3_ERROR("Light type unknown.");
    }
    
    // Return the newly created integrator
    return light;
}


Material * API::make_material( const ParamSet &ps_material)
{
    std::cout << ">>> Inside API::make_material()\n";
    material_type_t type = retrieve(ps_material, "type", material_type_t::flat);

    Material *material = nullptr;
    if(type == material_type_t::flat){
        material = create_flat_material(ps_material);
    }else if(type == material_type_t::blinn_phong){
        material = create_blinn_phong_material(ps_material);
    }else{
        RT3_ERROR("Uknown material type.");
    }

    // Return the newly created integrator
    return material;
}


GeometricPrimitive * API::make_geometric_primitive( 
        unique_ptr<Shape> &&shape, shared_ptr<Material> material ){

    std::cout << ">>> Inside API::make_primitive()\n";

    return new GeometricPrimitive(
        material,
        std::move(shape)
    );
}


shared_ptr<Primitive> API::make_primitive( const ParamSet& ps_accelerator, 
    vector<shared_ptr<BoundedPrimitive>>&& primitives){

    shared_ptr<Primitive> primitive = nullptr;

    accelerator_type_t type = retrieve(ps_accelerator, "type", accelerator_type_t::list);

    if(type == accelerator_type_t::list){
        primitive = shared_ptr<Primitive> (new PrimList(std::move(primitives)));
    }else if(type == accelerator_type_t::bvh){
        primitive = BVHAccel::build(std::move(primitives), 1);
    }else{
        RT3_ERROR("Unknown accerelator type.");
    }

    return primitive;
}


Shape * API::make_shape( const ParamSet& ps, shared_ptr<Transform> transform ){
    object_type_t type = retrieve(ps, "type", object_type_t::sphere);

    std::cout << ">>> Inside API::make_shape()\n";

    Shape *shape = nullptr;
    if(type == object_type_t::sphere){
        shape = create_sphere(ps, transform);
    }else{
        RT3_ERROR("Unknown object type.");
    }
    return shape;
}


vector<Shape *> API::make_triangles( shared_ptr<TriangleMesh> md){
    std::cout << ">>> Inside API::make_triangles()\n";
    vector<Shape *> shapes = create_triangle_list(md); 
    return shapes;
}

} // namespace rt3