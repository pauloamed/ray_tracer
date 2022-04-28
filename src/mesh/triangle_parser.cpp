#include "triangle_parser.h"


namespace rt3{

/// This function calls the basic tinyobjloader loading function and stores all the data into the tinyobjloader's internal data structures.
bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md )
{
    // Default load parameters
    const char* basepath = NULL;
    bool triangulate = true;

    // This is the tinyobj internal data structures.
    tinyobj::attrib_t attrib; // Hold all the data, such as vertex, normals, UV coords
    std::vector<tinyobj::shape_t> shapes; // Represents a collection of faces, grouped in a unit.
    std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

    // Warnings and Error values.
    std::string warn, err;

    
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
            basepath, triangulate);
    
    if(!warn.empty()) RT3_WARNING(warn);
    if(!err.empty()) RT3_ERROR(err);

    if (!ret) {
        RT3_WARNING("Failed to load/parse .obj.");
        return false;
    }

    // Let us now "convert" or "migrate" the data from tinyobjloader data structure into out mesh data.
    extract_obj_data( attrib, shapes, // TinyObjeLoader data structures (IN)
                      rvo, cn, fn,    // Mesh modifiers (IN)
                      md );           // Reference to the mesh data to fill in. (OUT)

    return true;
}



void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, /* OUT */ shared_ptr<TriangleMesh> md){
  
  // Logging 
  {
  std::cout << "-- SUMMARY of the OBJ file --\n";
  std::cout << "# of vertices  : " << (attrib.vertices.size()  / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size()   / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;
  std::cout << "# of shapes    : " << shapes.size()                 << std::endl;
  std::cout << "-----------------------------\n";
  }

  // Retrieve the complete list of vertices.
  retrieve_vertices(attrib, md);

  // Read the normals
  retrieve_normals(attrib, cn, fn, md);

  // Read the complete list of texture coordinates.
  // retrieve_textures(attrib, md);

  // Read mesh connectivity and store it as lists of indices to the real data.
  retrieve_shapes(shapes, rvo, md);

  // Logging
  {
  cout << "This is the list of indices: \n";

  cout << "   + Vertices [ ";
  std::copy( md->vertex_indices->begin(), md->vertex_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
  cout << "]\n";

  cout << "   + Normals [ ";
  std::copy( md->normal_indices->begin(), md->normal_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
  cout << "]\n";

  // cout << "   + UV coords [ ";
  // std::copy( md->uvcoord_indices->begin(), md->uvcoord_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
  // cout << "]\n";
  }
}


void retrieve_normals(const tinyobj::attrib_t& attrib, bool compute_normals, bool flip_normals, shared_ptr<TriangleMesh> md){
  auto n_normals{ attrib.normals.size()/3 };

  // TODO: flip normals if requested.
  real_type flip = ( flip_normals ) ? -1 : 1 ;

  // Do we need to compute the normals? Yes only if the user requeste or there are no normals in the file.
  if (compute_normals || n_normals == 0){
      RT3_ERROR("Not implemented.");
  }else {
    // Read normals from file. This corresponds to the entire 'for' below.
    // Traverse the normals read from the OBJ file.
    for ( auto idx_n{0u} ; idx_n < n_normals; idx_n++){
        // stringstream ss;
        // ss << "   n[" << static_cast<long>(idx_n) << "] = ( "
        //     << static_cast<double>(attrib.normals[3 * idx_n + 0]) << ", "
        //     << static_cast<double>(attrib.normals[3 * idx_n + 1]) << ", "
        //     << static_cast<double>(attrib.normals[3 * idx_n + 2]) << " )\n";
        // RT3_MESSAGE(ss.str());

        // Store the normal.
        auto normal = make_shared<Normal3f>(Normal3f{{ 
            attrib.normals[ 3 * idx_n + 0 ] * flip,
            attrib.normals[ 3 * idx_n + 1 ] * flip,
            attrib.normals[ 3 * idx_n + 2 ] * flip 
        }}.normalize());


        md->normals->push_back(normal);
    }
  }
}


void retrieve_vertices(const tinyobj::attrib_t& attrib, shared_ptr<TriangleMesh> md){
  auto n_vertices{ attrib.vertices.size()/3 };
  for ( auto idx_v{0u} ; idx_v < n_vertices; idx_v++)
  {
      // stringstream ss;
      // ss << "   v[" << static_cast<long>(idx_v) << "] = ( "
      //       << static_cast<double>(attrib.vertices[3 * idx_v + 0]) << ", "
      //       << static_cast<double>(attrib.vertices[3 * idx_v + 1]) << ", "
      //       << static_cast<double>(attrib.vertices[3 * idx_v + 2]) << " )\n";
      // RT3_MESSAGE(ss.str());

      // Store the vertex in the mesh data structure.

    auto vertex = make_shared<Point3f>(Point3f{{ 
    attrib.vertices[ 3 * idx_v + 0 ],
    attrib.vertices[ 3 * idx_v + 1 ],
    attrib.vertices[ 3 * idx_v + 2 ] 
    }});

    md->vertices->push_back(vertex);
  }
}


void retrieve_textures(const tinyobj::attrib_t& attrib, shared_ptr<TriangleMesh> md){
  auto n_texcoords{ attrib.texcoords.size()/2 };
  for ( auto idx_tc{0u} ; idx_tc < n_texcoords; idx_tc++){
    // stringstream ss;
    // ss << "   t[" << static_cast<long>(idx_tc) << "] = ( "
    //     << static_cast<double>(attrib.texcoords[2 * idx_tc + 0]) << ", "
    //     << static_cast<double>(attrib.texcoords[2 * idx_tc + 1]) << " )\n";
    // RT3_MESSAGE(ss.str());

    // Store the texture coords.

    auto texture = make_shared<Point2f>(Point2f{{ 
        attrib.texcoords[ 2 * idx_tc + 0 ],
        attrib.texcoords[ 2 * idx_tc + 1 ]
    }});    

    // md->uvcoords->push_back( texture );
  }
}


void retrieve_shapes(const std::vector<tinyobj::shape_t>& shapes, bool rvo, shared_ptr<TriangleMesh> md){
  auto n_shapes{ shapes.size() };
  // stringstream ss;
  md->n_triangles = 0; // We must reset this here.
  // In case the OBJ file has the triangles organized in several shapes or groups, we
  // ignore this and store all triangles as a single mesh dataset.
  // This is why we need to reset the triangle count here.
  for ( auto idx_s{0u} ; idx_s < n_shapes; idx_s++)
  {
      // ss << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
      // ss << "Size of shape["<< idx_s << "].mesh.indices: "
      //       << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size()) << '\n';
      // ss << "shape["<< idx_s << "].num_faces: "
      //     <<  static_cast<unsigned long>(shapes[idx_s].mesh.num_face_vertices.size()) << '\n';

      // For each face print out the indices lists.
      size_t index_offset = 0;
      // # of triangles for this "shape" (group).
      // NOTE that we are accumulate the number of triangles coming from the shapes present in the OBJ file.
      md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
      for ( auto idx_f{0} ; idx_f < md->n_triangles; idx_f++)
      {
          // Number of vertices per face (always 3, in our case)
          size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

          // ss << "  face[" << idx_f << "].fnum = "  << static_cast<unsigned long>(fnum) << '\n';

          // TODO: Invert order of vertices if flag is on. (DONE!)
          if ( rvo == true ) {
              // ss << "Reverse order\n";
              // For each vertex in the face print the corresponding indices
              for (int v = fnum-1; v >= 0 ; v--)
              {
                  tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                  // ss << "    face[" << idx_f << "].v[" << v << "].indices = "
                  //     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                  // Add the indices to the global list of indices we need to pass on to the mesh object.
                  md->vertex_indices->push_back( idx.vertex_index );
                  md->normal_indices->push_back( idx.normal_index );
                  // md->uvcoord_indices->push_back( idx.texcoord_index );
              }
          }
          else { // Keep the original vertex order
              // For each vertex in the face get the corresponding indices
              for (size_t v = 0; v < fnum; v++)
              {
                  tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                  // ss << "    face[" << idx_f << "].v[" << v << "].indices = "
                  //     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                  // Add the indices to the global list of indices we need to pass on to the mesh object.
                  // This goes to the mesh data structure.
                  md->vertex_indices->push_back( idx.vertex_index );
                  md->normal_indices->push_back( idx.normal_index );
                  // md->uvcoord_indices->push_back( idx.texcoord_index );
              }
          }

          // Advance over to the next triangle.
          index_offset += fnum;
      }
  }
  // RT3_MESSAGE(ss.str());
}

}