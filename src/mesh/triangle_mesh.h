#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../core/rt3-base.h"
#include "../core/transform.h"

namespace rt3{

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
  int n_triangles; //!< # of triangles in the mesh.
  bool backface_cull;

  // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
  shared_ptr<vector<int>> vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
  shared_ptr<vector<int>> normal_indices;  //!< The list of indices to the normals list, for each individual triangle.

  
  shared_ptr<vector<shared_ptr<Point3f>>> vertices;  //!< The 3D geometric coordinates
  shared_ptr<vector<shared_ptr<Normal3f>>> normals;  //!< The 3D normals.

  // Regular constructor
  TriangleMesh()
  :vertex_indices(new vector<int>), normal_indices(new vector<int>),
  vertices(new vector<shared_ptr<Point3f>>()), normals(new vector<shared_ptr<Normal3f>>())
  {/*empty*/};

  TriangleMesh(
    int n, bool bface,
    shared_ptr<vector<int>> indexes,
    shared_ptr<vector<shared_ptr<Point3f>>> vertexes,
    shared_ptr<vector<shared_ptr<Normal3f>>> normal
  ):n_triangles(n), backface_cull(bface), vertex_indices(indexes), normal_indices(indexes), 
  vertices(vertexes), normals(normal){}

  TriangleMesh(
    int n, bool bface,
    shared_ptr<vector<int>> vertex_indexes,
    shared_ptr<vector<int>> normal_indexes,
    shared_ptr<vector<shared_ptr<Point3f>>> vertexes,
    shared_ptr<vector<shared_ptr<Normal3f>>> normal
  ):n_triangles(n), backface_cull(bface), vertex_indices(vertex_indexes), normal_indices(normal_indexes), 
  vertices(vertexes), normals(normal){}

  shared_ptr<TriangleMesh> createCopy() const;

  TriangleMesh( const TriangleMesh& ) = delete;
  /// Assign operator disabled.
  TriangleMesh& operator=( const TriangleMesh & ) = delete;
  /// Move constructor.
  TriangleMesh( TriangleMesh && other ) = delete;

  void applyTransform(shared_ptr<Transform> t);
};


TriangleMesh *create_triangle_mesh(const ParamSet &ps);

Normal3f compute_normals();


}



#endif