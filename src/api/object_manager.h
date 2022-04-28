#ifndef OBJ_MAN_H
#define OBJ_MAN_H

#include <chrono>
#include <string>
#include "../core/rt3-base.h"
#include "../core/primitive.h"
#include "../core/transform.h"

namespace rt3{

  struct ObjectManager{
    struct Object{
        // the objects/primitives
        vector<tuple<ParamSet, shared_ptr<Material>, shared_ptr<Transform>>> primitives;
        vector<tuple<shared_ptr<TriangleMesh>, shared_ptr<Material>, shared_ptr<Transform>>> meshPrimitives;

        // the lights
        vector<ParamSet> lights;
    };

    // the objects/primitives
    vector<tuple<ParamSet, shared_ptr<Material>, shared_ptr<Transform>>> globalPrimitives;
    vector<tuple<shared_ptr<TriangleMesh>, shared_ptr<Material>, shared_ptr<Transform>>> globalMeshPrimitives;

    // the lights
    vector<ParamSet> globalLights;



    map<string, shared_ptr<Object>> namedObjects;
    string currObject;

    ObjectManager():currObject(""){ }

    void addLight(const ParamSet &ps);
    void addSimplePrimitive(const ParamSet &ps, shared_ptr<Material> mat, shared_ptr<Transform> transform);
    void addMeshPrimitive(const shared_ptr<TriangleMesh> mesh, shared_ptr<Material> mat, shared_ptr<Transform> transform);

    bool isBuilding();
    void startBuilding(string name);
    void finishBuilding();


    void instantiate(string name, shared_ptr<Transform> transform);
    void reset();
};
}


#endif