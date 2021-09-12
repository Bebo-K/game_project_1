#ifndef SPRITE_WIDGET_H
#define SPRITE_WIDGET_H

#include "../widget.h"
#include "../../gfx/ui_sprite.h"

namespace UI{

class SpriteComponent: public WidgetComponent{
    Sprite sprite;
    public:
    SpriteComponent(char* filename);
    SpriteComponent(Texture sprite_tex);

    void OnPaint(Widget *w);
    void OnResize(Widget *w);
};

}


#endif