#include "graphics_managers.h"

namespace rt3{

/// Collection of data related to a Graphics state, such as current material, lib of material, etc.

stack<shared_ptr<Transform>> ctm_states;

void GraphicsState::MatrixTransformationState::persistMatrix(){
    ctm_states.push(make_shared<Transform>(*ctm_states.top()));
}

void GraphicsState::MatrixTransformationState::rollbackMatrix(){
    ctm_states.pop();
}

shared_ptr<Transform> GraphicsState::MatrixTransformationState::getCTM(){
    return ctm_states.top();
}  

void GraphicsState::MatrixTransformationState::set(shared_ptr<Transform> t){
    ctm_states.top() = t;
}  

void GraphicsState::MatrixTransformationState::transformCTM(Transform &t){
    (*ctm_states.top()) = ctm_states.top()->update(t);
}


void GraphicsState::setMaterial(shared_ptr<Material> mat){
    states.top().material = mat;
}

shared_ptr<Material> GraphicsState::material(){
    return states.top().material;
}

GraphicsState::MatrixTransformationState& GraphicsState::mts(){
    return states.top().mts;
}

void GraphicsState::persistState(){
    // novo estado na ovai ter tudo na pilha nao
    auto newInternal = InternalState(states.top().material, make_shared<Transform>(*states.top().mts.getCTM()));
    states.push(newInternal);
}

void GraphicsState::rollbackState(){
    states.pop();
}

}