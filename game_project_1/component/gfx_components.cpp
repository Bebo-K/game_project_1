#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/game/entity.hpp>

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

ModelSet::ModelSet(){}
ModelSet::~ModelSet(){Clear();}


void SpriteSet::Draw(Camera* cam){
    for(Sprite* s: (*this)){
        s->Draw();
    }
}
SpriteSet::~SpriteSet(){Clear();}
