#include "standard_widgets.h"
#include "../../gfx/font_manager.h"

using namespace UI;


MenuBackground::MenuBackground():Widget("background"){
    layout.center = {UI_WIDTH/2,UI_HEIGHT/2};
    layout.width = UI_WIDTH;
    layout.height = UI_HEIGHT;
    sprites.Resize(1);
        sprites.Set(0,new Sprite("low_effort_banner"));
        sprites[0]->width = UI_WIDTH;
        sprites[0]->height = UI_HEIGHT;
}

MenuBackground::~MenuBackground(){}

void SimpleButtonHighlightEffect(Widget* w){
    SimpleButton* button = (SimpleButton*)w;
    vec4 old_color = button->rects[0]->color;
    old_color.x += 0.1f;
    old_color.y += 0.1f;
    old_color.z += 0.1f;
    old_color.w += 0.1f;
    button->rects[0]->color = old_color;
}
void SimpleButtonUnhighlightEffect(Widget* w){
    SimpleButton* button = (SimpleButton*)w;
    vec4 old_color = button->rects[0]->color;
    old_color.x -= 0.1f;
    old_color.y -= 0.1f;
    old_color.z -= 0.1f;
    old_color.w -= 0.1f;
    button->rects[0]->color = old_color;
}
bool SimpleButtonSelectedInputCallback(Widget* w, Input::Event event_type){
    SimpleButton* button = (SimpleButton*)w;
    if(event_type == Input::A && Controller::GetButton(Controller::A).IsJustPressed()){
        button->select_callback();
        return true;
    }
    return false;
}
void SimpleButtonOnClickCallback(Widget* w){
    SimpleButton* button = (SimpleButton*)w;
    button->select_callback();
}

SimpleButton::SimpleButton(char* name, wchar* label, float w, float h, vec4 color, void (*callback)()):Widget(name){
    FontID simple_button_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    layout.width = w;
    layout.height = h;

    rects.Resize(1);
    rects.Set(0,new UI_Rect());
        rects[0]->color = color;
        rects[0]->rect = layout.Rect();
        
    texts.Resize(1);
    texts.Set(0,new DrawableText(label,simple_button_font));
        texts[0]->x = layout.center.x - (texts[0]->w/2);
        texts[0]->y = layout.center.y - (texts[0]->h/2);

    clickable = new WidgetClickInfo();
        clickable->onClickAction = SimpleButtonOnClickCallback;

    selectable = new WidgetSelectInfo();
        selectable->onHighlightEffect = SimpleButtonHighlightEffect;
        selectable->onStopHighlightEffect = SimpleButtonUnhighlightEffect;
        selectable->onSelectedInput = SimpleButtonSelectedInputCallback;
    
    select_callback = callback;
}

SimpleButton::~SimpleButton(){}
