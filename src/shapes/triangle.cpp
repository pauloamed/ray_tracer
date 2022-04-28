#include "triangle.h"


namespace rt3{

bool Triangle::_intersect(const Ray &r, real_type &t, real_type &u, real_type &v) const{
  Vector3f edge[2] = {*vert[1] - *vert[0], *vert[2] - *vert[0]};

	Vector3f h = r.d.cross(edge[1]);
	
	auto a = edge[0] * h;
	if(abs(a) < EPS) return false; // This ray is parallel to this triangle.
	
	real_type f = 1 / a;
	Vector3f s = r.o - *vert[0];

	u = f * (s * h); // coordenada baricentrica u
	if(u < 0.0 || u > 1.0) return false; 

	Vector3f q = s.cross(edge[0]);
	v = f * (r.d * q); // coordenada baricentrica v
	if (v < 0.0 || u + v > 1.0) return false; 

	t = f * (edge[1] * q);
	if (t < EPS) return false;

	return true;
}

bool Triangle::intersect_p(const Ray &r, real_type maxT) const{
	real_type t, u, v;
	auto flag = _intersect(r, t, u, v);
	if(!flag || t > maxT) return false;
	else return true;
}

bool Triangle::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect) const{
	real_type t, u, v;
	if(!_intersect(r, t, u, v)) return false;

	Point3f contact = ((*vert[1] * u) + (*vert[2] * v) + (*vert[0] * (1 - u - v)));
	// normal tem que ser uma media das normais
	// assert((contact - (r.o + r.d * t)).getNorm() < EPS);

	Normal3f finalNormal = (*n[1] * u) + (*n[2] * v) + (*n[0] * (1 - u - v));
	finalNormal = finalNormal.normalize();

	if(mesh->backface_cull && finalNormal * r.d > 0) return false;


	isect = unique_ptr<ObjSurfel>(new ObjSurfel(contact, finalNormal, r.d * -1, t));	
	return true;
}

Bounds3f Triangle::computeBounds() const{
	return Bounds3f::createBox({*vert[0], *vert[1], *vert[2]});
}

vector<Shape *> create_triangle_list( shared_ptr<TriangleMesh> mesh){
	// List of shapes (triangles) we need to return to the client.
	vector<Shape *> tris;
	// Create the triangles, which are just references to the mesh database.
	for ( int i = 0 ; i < mesh->n_triangles ; ++i )
		tris.push_back(new Triangle( mesh, i ) );

	return tris;
}

}
