#include "triangle_mesh.h"

namespace rt3{

TriangleMesh *create_triangle_mesh(const ParamSet &ps){

  auto n = retrieve(ps, "ntriangles", 1);
  auto backface_cull = retrieve(ps, "backface_cull", false);
  auto indices = retrieve(ps, "indices", shared_ptr<vector<int>>(new vector<int>));

  auto vertices = retrieve(ps, "vertices", shared_ptr<vector<shared_ptr<Point3f>>>(new vector<shared_ptr<Point3f>>)); 
  auto normals = retrieve(ps, "normals", shared_ptr<vector<shared_ptr<Normal3f>>>(new vector<shared_ptr<Normal3f>>));

  if(indices->size() % 3 != 0 || (int) indices->size() != n * 3){
    RT3_ERROR("Indices size doesnt match num. of triangles!");
  }
 
  if(retrieve(ps, "reverse_vertex_order", false)){
    auto it = indices->begin();
    while(it != indices->end()){
      reverse(it, it + 3);
      it += 3;
    }
  }

  if(ps.count("normals") == 0 || retrieve(ps, "compute_normals", false)){
    RT3_ERROR("Not implemented.");
  }

  auto tm = new TriangleMesh(n, backface_cull, indices, vertices, normals);


  return tm;
}

Normal3f compute_normals(shared_ptr<Point3f> a, shared_ptr<Point3f> b, shared_ptr<Point3f> c){
  Vector3f edges[2] = {*a - *b, *c - *a};
  return edges[0].cross(edges[1]);
}

shared_ptr<TriangleMesh> TriangleMesh::createCopy() const{
  shared_ptr<vector<shared_ptr<Point3f>>> newVertices = make_shared<vector<shared_ptr<Point3f>>>(vector<shared_ptr<Point3f>>());
  shared_ptr<vector<shared_ptr<Normal3f>>> newNormals = make_shared<vector<shared_ptr<Normal3f>>>(vector<shared_ptr<Normal3f>>());

  for(auto &v : *vertices) newVertices->push_back(make_shared<Point3f>(*v));
  for(auto &n : *normals) newNormals->push_back(make_shared<Normal3f>(*n));

  return make_shared<TriangleMesh>(
    n_triangles,
    backface_cull,
    vertex_indices,
    normal_indices,
    newVertices,
    newNormals
  );  
}


void TriangleMesh::applyTransform(shared_ptr<Transform> t){
  for(auto v : *vertices) *v = t->apply(*v);
  for(auto n : *normals) *n = t->apply(*n);
}


}