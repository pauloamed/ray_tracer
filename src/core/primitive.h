#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "math_base.h"
#include "shape.h"


namespace rt3{

class Primitive {
public:
	virtual ~Primitive(){};
	virtual bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const = 0;
	virtual bool intersect_p( const Ray& r, real_type maxT ) const = 0;
};

class BoundedPrimitive : public Primitive{
protected:
	Bounds3f boundingBox;
public:
	BoundedPrimitive(Bounds3f bb):boundingBox(bb){}
	virtual ~BoundedPrimitive(){}
	Bounds3f getBoundingBox() { return boundingBox; }
};


class AggregatePrimitive : public BoundedPrimitive{
public:
	vector<shared_ptr<BoundedPrimitive>> primitives;

	AggregatePrimitive(vector<shared_ptr<BoundedPrimitive>> &&prim):
		BoundedPrimitive(prim[0]->getBoundingBox()), primitives(std::move(prim)){

		for(size_t i = 1; i < primitives.size(); ++i){
			boundingBox = Bounds3f::unite(boundingBox, primitives[i]->getBoundingBox());
		}
	}

	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:

	PrimList(vector<shared_ptr<BoundedPrimitive>> &&prim):AggregatePrimitive(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

};


class BVHAccel : public AggregatePrimitive{

private:
	static vector<shared_ptr<BVHAccel>> createLeaves(vector<shared_ptr<BoundedPrimitive>> &&prim, size_t leafSize);
	static bool boundedComp(shared_ptr<BoundedPrimitive> a, shared_ptr<BoundedPrimitive> b);

public:
	BVHAccel(vector<shared_ptr<BoundedPrimitive>> &&prim):AggregatePrimitive(std::move(prim)){}

	~BVHAccel(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

	static shared_ptr<BVHAccel> build(vector<shared_ptr<BoundedPrimitive>> &&prim, size_t leafSize);

};


class GeometricPrimitive : public BoundedPrimitive,  public std::enable_shared_from_this<GeometricPrimitive>{
public:
	shared_ptr<Material> material;
	unique_ptr<Shape> shape;

	GeometricPrimitive(shared_ptr<Material> mat, unique_ptr<Shape> &&s):
		BoundedPrimitive(s->computeBounds()), material(mat), shape(std::move(s)){}

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, shared_ptr<ObjSurfel> &isect ) const override;

	shared_ptr<Material> get_material() const{  return material; }
};

} // namespace rt3



#endif