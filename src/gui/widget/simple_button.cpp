#include "simple_button.h"

#include "../../gfx/font_manager.h"
#include "rect_component.h"
#include "button_component.h"
#include "text_component.h"

//using namespace UI;

UI::Widget* UI::BuildSimpleButton(char* name, char* label, float w, float h, vec4 color, void (*callback)()){
    FontID simple_button_font = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",32);
    Widget* ret = new Widget(name);
    ret->layout.SetSize(w,h);
    ret->components.Add(new ButtonComponent(callback));
    ret->components.Add(new RectComponent(color));
    ret->components.Add(new TextComponent(label,simple_button_font));

    return ret;
}

