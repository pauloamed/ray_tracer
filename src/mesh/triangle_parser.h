#ifndef TRIANGLE_PARSER_H
#define TRIANGLE_PARSER_H

#include "../ext/tiny_obj_loader.h"

#include "triangle_mesh.h"


namespace rt3{

// Loads obj file at filename and then calls extract_obj_data
bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md );


// Extracts data from attrib and saves into md
// Calls retrieve functions for each step
void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, /* OUT */ shared_ptr<TriangleMesh> md );


void retrieve_normals(const tinyobj::attrib_t& attrib, bool compute_normals, bool flip_normals, shared_ptr<TriangleMesh> md);


void retrieve_vertices(const tinyobj::attrib_t& attrib, shared_ptr<TriangleMesh> md);


void retrieve_textures(const tinyobj::attrib_t& attrib, shared_ptr<TriangleMesh> md);


void retrieve_shapes(const std::vector<tinyobj::shape_t>& shapes, bool rvo, shared_ptr<TriangleMesh> md);
}

#endif