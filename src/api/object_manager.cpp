#include "object_manager.h"

namespace rt3{

void ObjectManager::addLight(const ParamSet &ps){
    if(isBuilding()){
      namedObjects[currObject]->lights.push_back(ps);
    }else{
      globalLights.push_back(ps);
    }
}

void ObjectManager::addSimplePrimitive(const ParamSet &ps, shared_ptr<Material> mat, shared_ptr<Transform> transform){
    if(isBuilding()){
      namedObjects[currObject]->primitives.push_back({ps, mat, transform});
    }else{
      globalPrimitives.push_back({ps, mat, transform});
    } 
}

void ObjectManager::addMeshPrimitive(const shared_ptr<TriangleMesh> mesh, shared_ptr<Material> mat, shared_ptr<Transform> transform){
    if(isBuilding()){
      namedObjects[currObject]->meshPrimitives.push_back({mesh, mat, transform});
    }else{
      globalMeshPrimitives.push_back({mesh, mat, transform});
    } 
}

bool ObjectManager::isBuilding(){
    return currObject != "";
}

void ObjectManager::startBuilding(string name){
    currObject = name;
    namedObjects[name] = make_shared<Object>(Object());
}

void ObjectManager::finishBuilding(){
    currObject = "";
}


void ObjectManager::instantiate(string name, shared_ptr<Transform> transform){
    for(auto ps : namedObjects[name]->lights){
        globalLights.push_back(ps);
    }
    for(auto [ps, mat, objTransform] : namedObjects[name]->primitives){
        globalPrimitives.push_back({ps, mat, make_shared<Transform>(transform->update(*objTransform))});
    }
    for(auto [mesh, mat, objTransform] : namedObjects[name]->meshPrimitives){
        globalMeshPrimitives.push_back({mesh, mat, make_shared<Transform>(transform->update(*objTransform))});
    }
}

void ObjectManager::reset(){

}

};