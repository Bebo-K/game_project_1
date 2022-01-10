#include "standard_widgets.h"

#include "../../gfx/font_manager.h"
#include "component_rect.h"
#include "component_text.h"

using namespace UI;


SimpleButton::SimpleButton(char* name, char* label, float w, float h, vec4 color, void (*callback)()):Widget(name){
    FontID simple_button_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    layout.SetSize(w,h);
    components.Add(new RectComponent(color));
    components.Add(new TextComponent(label,simple_button_font));

    ClickableComponent* clickable = new ClickableComponent();
        clickable->onClickAction = callback;

    SelectableComponent* selectable = new SelectableComponent();
        selectable->onSelectAction = callback;

    components.Add(clickable);
    components.Add(selectable);
}

SimpleButton::~SimpleButton(){}

SelectableComponent* SimpleButton::GetSelectionInfo(){
    return (SelectableComponent*)components[2];
}
ClickableComponent* SimpleButton::GetClickInfo(){
    return (ClickableComponent*)components[3];
}