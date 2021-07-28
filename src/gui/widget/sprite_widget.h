#ifndef SPRITE_WIDGET_H
#define SPRITE_WIDGET_H

#include "../widget.h"
#include "../../gfx/ui_sprite.h"

class SpriteWidget: public Widget{
    Sprite sprite;
    public:
    SpriteWidget(char* filename);
    SpriteWidget(Texture sprite_tex);
    SpriteWidget(Texture sprite_tex, int w, int h);
    ~SpriteWidget();

    void OnPaint();
    void OnResize();
};

















#endif