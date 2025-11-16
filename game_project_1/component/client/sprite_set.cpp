#include <game_project_1/component/client/sprite_set.hpp>


SpriteSet::SpriteSet(Transform* parent):Drawable(parent){}
SpriteSet::~SpriteSet(){Clear();}

void SpriteSet::Draw(Camera* cam){
    for(Sprite* s: (*this)){
        s->Draw();
    }
}

Component* SpriteSet::Clone(ComponentParentContext context){
    SpriteSet* copy = new SpriteSet(context.transform);
    copy->layer = layer;
    copy->hidden = hidden;
    copy->shader = copy->shader;
    for(Sprite* s: (*this)){
        Sprite* s2 = new (copy->Allocate()) Sprite(s->texture);
        s->CopyTo(s2);
    }
    return copy;
}
void SpriteSet::Clear(){this->Pool::Clear();}
