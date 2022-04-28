#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../core/shape.h"
#include "../mesh/triangle_mesh.h"

namespace rt3{

/// Represents a single triangle.
class Triangle : public Shape {
private:
  shared_ptr<const Point3f> vert[3];
  shared_ptr<const Normal3f> n[3];

  shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.



public:
  // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
  Triangle( shared_ptr<TriangleMesh> mesh, int tri_id)
    : Shape(), mesh{mesh}
  {

    auto &v_indexes = *(mesh->vertex_indices);
    auto &n_indexes = *(mesh->normal_indices);
    
    auto &vertices = *(mesh->vertices);
    auto &normals = *(mesh->normals);
    // auto &uvcoords = *(mesh->uvcoords);

    // This is just a shortcut to access this triangle's data stored in the mesh database.
    for(int i = 0; i < 3; ++i){
      vert[i] = vertices[v_indexes[ 3 * tri_id + i]];
      n[i] = normals[n_indexes[ 3 * tri_id + i]];
    }

  }


  /// Return the triangle's bounding box.
  Bounds3f computeBounds() const override;
  /// The regular intersection methods, as defined in the Shape parent class.
  bool _intersect(const Ray &r, real_type &t, real_type &u, real_type &v) const;

  bool intersect_p(const Ray &r, real_type maxT) const override;
  bool intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const override;

  /// This friend function helps us debug the triangles, if we want to.
  friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
};


/// This function creates the internal data structure, required by the RT3.
vector<Shape *> create_triangle_list( shared_ptr<TriangleMesh> mesh);

}

#endif