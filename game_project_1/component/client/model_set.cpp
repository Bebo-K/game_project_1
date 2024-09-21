#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/core/entity.hpp>

ModelSet::ModelSet(){}
ModelSet::~ModelSet(){Clear();}

Model* ModelSet::Add(ModelID model_id){
    return new (Allocate()) Model(model_id);
}

void ModelSet::Draw(Camera* cam){
    cam->view_matrix.translate(x,y,z);
    cam->view_matrix.rotate(rotation);
    cam->view_matrix.scale(scale);
    for(Model* m: (*this)){
        m->Draw(cam);
    }
}

void ModelSet::SetPosition(vec3 pos){
    x=pos.x;y=pos.y;z=pos.z;
}

void ModelSet::StartAnimation(char* animation_name){
    for(Model* m: (*this)){
        m->StartAnimation(animation_name);
    }
}

void ModelSet::QueueAnimation(char* animation_name){
    for(Model* m: (*this)){
        m->SetQueuedAnimation(animation_name);
    }
}

void ModelSet::StopAnimation(){
    for(Model* m: (*this)){
        m->StopAnimation();
    }
}

void ModelSet::PauseAnimation(){
    for(Model* m: (*this)){
        m->PauseAnimation();
    }
}

//Modifies the play speed of the current clip only 
void ModelSet::SetAnimationTimescale(float timescale){
    for(Model* m: (*this)){
        m->SetAnimationTimescale(timescale);
    }
}

Component* ModelSet::Clone(){
    ModelSet* copy = new ModelSet();
    copy->SetPosition({x,y,z});
    copy->rotation =rotation;
    copy->scale = scale;
    copy->layer = layer;
    copy->hidden = hidden;
    copy->shader_name = copy->shader_name;
    for(Model* m: (*this)){copy->Add(m->type_id);}
    return copy;
}
void ModelSet::Clear(){this->Pool::Clear();}

