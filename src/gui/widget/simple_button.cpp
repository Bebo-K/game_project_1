#include "simple_button.h"

#include "../../gfx/font_manager.h"
#include "rect_component.h"
#include "button_component.h"
#include "text_component.h"

using namespace UI;

Widget* BuildSimpleButton(char* name, char* label, int w, int h, vec4 color, void (*callback)()){
    FontID simple_button_font = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",32);
    Widget* ret = new Widget(name);
    ret->layout.w=w;
    ret->layout.h=h;

    ret->components.Add(RectComponent(color));



}