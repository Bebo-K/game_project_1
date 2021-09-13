#include "sprite_component.h"

using namespace UI;

SpriteComponent::SpriteComponent(char* filename): sprite(TextureManager::Get(filename)){}

SpriteComponent::SpriteComponent(Texture sprite_tex):sprite(sprite_tex){}

void SpriteComponent::OnPaint(Widget* w){
    sprite.Draw();
}
void SpriteComponent::OnResize(Widget* w){
    sprite.x = w->layout.x;
    sprite.y = w->layout.y;
    sprite.scale = {(float)w->layout.w/sprite.width,(float)w->layout.h/sprite.height};
}