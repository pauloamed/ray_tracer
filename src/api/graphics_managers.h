#ifndef GRAPHICS_MAN
#define GRAPHICS_MAN

#include <chrono>
#include <string>
#include "../core/rt3-base.h"
#include "../core/primitive.h"
#include "../core/transform.h"

namespace rt3{

/// Collection of data related to a Graphics state, such as current material, lib of material, etc.
struct GraphicsState{
  struct MatrixTransformationState{
      stack<shared_ptr<Transform>> ctm_states;

      MatrixTransformationState(){
        ctm_states.push(shared_ptr<Transform>(new Transform()));
      }

      MatrixTransformationState(shared_ptr<Transform> t){
        ctm_states.push(t);
      }

      void persistMatrix();

      void rollbackMatrix();

      shared_ptr<Transform> getCTM();

      void set(shared_ptr<Transform> t);

      void transformCTM(Transform &t);
  };

  struct InternalState{       
      MatrixTransformationState mts;
      shared_ptr<Material> material;

      InternalState():material(nullptr){ }

      InternalState(shared_ptr<Material> mat, shared_ptr<Transform> t):
          mts(t), material(mat){ }
  
  };

  stack<InternalState> states;

  GraphicsState(){
      states.push(InternalState());
  }

  void setMaterial(shared_ptr<Material> mat);

  shared_ptr<Material> material();

  MatrixTransformationState& mts();

  void persistState();

  void rollbackState();

};

struct GraphicsContext{
    map<string,shared_ptr<Material>> named_materials;
    map<string,shared_ptr<Transform>> coords_systems;
    map<string,shared_ptr<TriangleMesh>> meshes;
};

}

#endif