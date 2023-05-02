#include <game_project_1/component/client/sprite_set.hpp>


SpriteSet::SpriteSet(){}
SpriteSet::~SpriteSet(){Clear();}

void SpriteSet::Draw(Camera* cam){
    for(Sprite* s: (*this)){
        s->Draw();
    }
}
void SpriteSet::SetPosition(vec3 pos){
    x=pos.x;y=pos.y;z=pos.z;
}

Component* SpriteSet::Clone(){
    SpriteSet* copy = new SpriteSet();
    copy->SetPosition({x,y,z});
    copy->rotation =rotation;
    copy->scale = scale;
    copy->layer = layer;
    copy->hidden = hidden;
    copy->shader_name = copy->shader_name;
    for(Sprite* s: (*this)){copy->Add(s);}//We need to copy the sprite right?
    return copy;
}
void SpriteSet::Clear(){this->Pool::Clear();}
