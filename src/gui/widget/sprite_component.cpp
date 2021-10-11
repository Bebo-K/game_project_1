#include "sprite_component.h"

using namespace UI;

SpriteComponent::SpriteComponent(char* sprite_uri): sprite(TextureManager::Get(sprite_uri,true)){}

SpriteComponent::SpriteComponent(Texture sprite_tex):sprite(sprite_tex){}

SpriteComponent::~SpriteComponent(){}

void SpriteComponent::OnPaint(Widget* w){
    sprite.Draw();
}
void SpriteComponent::OnResize(Widget* w){
    sprite.x = w->layout.GetRect().x;
    sprite.y = w->layout.GetRect().y;
    sprite.scale = {w->layout.w/sprite.width, w->layout.h/sprite.height};
}