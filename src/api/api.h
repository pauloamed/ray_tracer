#ifndef API_H
#define API_H

#include <chrono>
#include <string>
#include "../core/rt3-base.h"
#include "../core/primitive.h"
#include "../core/transform.h"
#include "object_manager.h"
#include "graphics_managers.h"

#include "../mesh/triangle_parser.h"

//=== API Macro definitions

/// Check whether the current state has been intialized.
#define VERIFY_INITIALIZED( func_name )                             \
    if ( curr_state == APIState::Uninitialized ) {                  \
        RT3_ERROR( std::string{"API::init() must be called before "} +  \
               std::string{func_name} +                             \
               std::string{"(). Ignoring."} );                      \
        return;                                                     \
    } else /* Empty so that it receives the semicolon palace after this macro. */

/// Check whether the current state corresponds to setup section.
#define VERIFY_SETUP_BLOCK( func_name )                             \
    VERIFY_INITIALIZED( func_name )                                 \
    if ( curr_state == APIState::WorldBlock ) {                     \
        RT3_ERROR( std::string{"Rendering setup cannot happen inside "} +\
               std::string{"World definition block; "} +            \
               std::string{func_name} +                             \
               std::string{"() not allowed. Ignoring"} );           \
        return;                                                     \
    } else /* Empty so that it receives the semicolon palace after this macro. */

/// Check whether the current state corresponds to the world section.
#define VERIFY_WORLD_BLOCK( func_name )                             \
    VERIFY_INITIALIZED( func_name )                                 \
    if ( curr_state == APIState::SetupBlock ) {                     \
        RT3_ERROR( std::string{"Scene description must happen inside "}+\
               std::string{"World Definition block; "} +            \
               std::string{func_name} +                             \
               std::string{"() not allowed. Ignoring"} );           \
        return;                                                     \
    } else /* Empty so that it receives the semicolon palace after this macro. */


namespace rt3 {
    /// Collection of objects and diretives that control rendering, such as camera, lights, prims.
    struct RenderOptions
    {
        // the Film
        std::string film_type{"image"}; // The only type available.
        ParamSet film_ps;
        
        /// the Camera
        camera_type_t camera_type{camera_type_t::perspective};
        ParamSet camera_ps;

        /// the Look At
        ParamSet look_at_ps;

        /// the integrator
        ParamSet integrator_ps;
        
        /// the Background
        bg_type_t bkg_type{bg_type_t::colors}; // "image", "interpolated"
        ParamSet bkg_ps;

        // Accel
        ParamSet accelerator_ps;
    };





    /// Static class that manages the render process
    class API
    {
        public:
            static GraphicsState curr_GS;

            static ObjectManager objM;
    
            /// Defines the current state the API may be at a given time
            enum class APIState { Uninitialized,    //!< Initial state, before parsing.
                                  SetupBlock,       //!< Parsing the render setup section.
                                  WorldBlock };     //!< Parsing the world's information section.


            /// Stores the running options collect in main().
            static RunningOptions curr_run_opt;
            static GraphicsContext curr_GC;
        private:
            /// Current API state
            static APIState curr_state;
            /*
             * The unique pointer below is useful to support various calls to init()-run()-clean-up(),
             * in case we want to process several input files in a single run of the system.
             */
            /// Unique infrastructure to render a scene (camera, integrator, etc.).
            static unique_ptr< RenderOptions > render_opt;
            // [NO NECESSARY IN THIS PROJECT]
            // /// The current GraphicsState
            // static GraphicsState curr_GS;
            // [NOT NECESSARY IN THIS PROJECT]
            // /// Pointer to the scene. We keep it as parte of the API because it may be reused later [1] Create the integrator.
            // static unique_ptr< Scene > the_scene;
            

        //== Private methods
        private:
            // === Helper functions.
            ///
            static Film * make_film( const ParamSet& ps );

            static Integrator * make_integrator( const ParamSet& ps, unique_ptr<Camera> &&camera );

            static Background * make_background( const ParamSet& ps );

            static Material * make_material( const ParamSet& ps );

            static Shape * make_shape( const ParamSet& p, shared_ptr<Transform> transform );
            static vector<Shape *> make_triangles( shared_ptr<TriangleMesh> );

            static Light * make_light( const ParamSet& ps, Bounds3f worldBox );

            static GeometricPrimitive * make_geometric_primitive( 
                unique_ptr<Shape> &&shape, shared_ptr<Material> material );

            static Camera * make_camera( const ParamSet& ps_camera, 
                const ParamSet& ps_look_at, unique_ptr<Film>&& the_film );

            static shared_ptr<Primitive> make_primitive( const ParamSet& ps_accelerator, 
                vector<shared_ptr<BoundedPrimitive>>&& primitives);

        public:
            //=== API function begins here.
            static void init_engine( const RunningOptions& );
            static void run( void );
            static void clean_up( void );
            static void clean_world_elements( void );
            static void reset_engine( void );

            static void integrator( const ParamSet& ps );
            static void film( const ParamSet& ps );
            static void lookat( const ParamSet& ps );
            static void camera( const ParamSet& ps );
            static void background( const ParamSet& ps );

            static void accelerator( const ParamSet& ps );

            static void create_named_material( const ParamSet& ps );
            static void named_material( const ParamSet& ps );

            static void pop_GS( void );
            static void push_GS( void );
            static void pop_CTM( void );
            static void push_CTM( void );            

            static void translate( const ParamSet& ps );
            static void rotate( const ParamSet& ps );
            static void scale( const ParamSet& ps );
            static void identity( void );
            static void save_coord_system( const ParamSet& ps );
            static void restore_coord_system( const ParamSet& ps );
            
            static void material( const ParamSet& ps );
            static void object( const ParamSet& ps );
            static void light( const ParamSet& ps );
            
            static void start_obj_instance( const ParamSet& ps );
            static void instantiate_obj( const ParamSet& ps );
            static void finish_obj_instance( void );
            
            static void world_begin( void );
            static void world_end( void );
    };
}  // namespace rt3

#endif // API_H
