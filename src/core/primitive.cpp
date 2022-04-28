#include "primitive.h"

namespace rt3{

bool GeometricPrimitive::intersect_p( const Ray& r, real_type maxT ) const{
    return shape->intersect_p(r, maxT); 
}

bool GeometricPrimitive::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect ) const{
    if(shape->intersect(r, isect)){
        auto selfRef = shared_ptr<const GeometricPrimitive>(shared_from_this());
        isect->setPrimitive(selfRef);
        return true;
    }else return false; 
}

bool BVHAccel::intersect_p( const Ray& r, real_type maxT ) const{
    if(boundingBox.intersect_p(r, maxT)){
        for(auto &prim : primitives){
            if(prim->intersect_p(r, maxT)) return true;
        }
        return false;
    }else return false;
}

bool BVHAccel::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect ) const{
    pair<real_type, real_type> t;
    if(!boundingBox.box_intersect(r, t)) return false;

    shared_ptr<ObjSurfel> currIsect(nullptr);
    for(auto &prim : primitives)
    {   
        if(prim->intersect(r, currIsect)){
            if(isect == nullptr || currIsect->t < isect->t){
                isect = currIsect;
            }
        }
    }
    return (isect != nullptr);
}

bool PrimList::intersect(const Ray &r, shared_ptr<ObjSurfel> &isect ) const{
    shared_ptr<ObjSurfel> currIsect(nullptr);
    for(auto &prim : primitives)
    {   
        if(prim->intersect(r, currIsect)){
            if(isect == nullptr || currIsect->t < isect->t){
                isect = currIsect;
                isect->setPrimitive(std::dynamic_pointer_cast<GeometricPrimitive>(prim));
            }
        }
    }
    return (isect != nullptr);
}

bool PrimList::intersect_p(const Ray& r, real_type maxT) const{
    for(auto &prim : primitives)
    {
        if(prim->intersect_p(r, maxT)) return true;
    }
    return false;
}

bool BVHAccel::boundedComp(shared_ptr<BoundedPrimitive> a, shared_ptr<BoundedPrimitive> b){
    return a->getBoundingBox().minPoint.at(0) < b->getBoundingBox().minPoint.at(0);
}

vector<shared_ptr<BVHAccel>> BVHAccel::createLeaves(vector<shared_ptr<BoundedPrimitive>> &&prim, size_t leafSize){
    vector<shared_ptr<BoundedPrimitive>> startList = std::move(prim);

    sort(startList.begin(), startList.end(), boundedComp);

    vector<shared_ptr<BVHAccel>> leaves;
    for(size_t i = 0; i < startList.size(); i += leafSize){

        vector<shared_ptr<BoundedPrimitive>> currLeaf;

        for(size_t j = i; j - i < leafSize; ++j){
            currLeaf.push_back(startList[j]);
        }

        shared_ptr<BVHAccel> newNode{ new BVHAccel(std::move(currLeaf)) };
        leaves.push_back(newNode);
    }
    
    return leaves;
}

shared_ptr<BVHAccel> BVHAccel::build(vector<shared_ptr<BoundedPrimitive>> &&prim, size_t leafSize){

    auto currList = createLeaves(std::move(prim), leafSize);

    while(currList.size() != 1){
        vector<shared_ptr<BVHAccel>> nextList;
        for(size_t i = 0; i < currList.size(); i += 2){

            shared_ptr<BVHAccel> newNode = nullptr;

            if(i + 1 == currList.size()){
                newNode = shared_ptr<BVHAccel>(new BVHAccel({currList[i]}));
            }else{
                newNode = shared_ptr<BVHAccel>(new BVHAccel({currList[i], currList[i + 1]}));
            }

            nextList.push_back(newNode);
        }
        currList = std::move(nextList);
    }
    return currList[0];
}


}