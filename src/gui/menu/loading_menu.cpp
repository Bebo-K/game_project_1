#include "loading_menu.h"
#include "../widget/sprite_component.h"

using namespace UI;

LoadingMenu::LoadingMenu(Layout* parent) : Menu(parent){
    id = Menu::LOADING;
    h_status_text = nullptr;
}

void LoadingMenu::OnLoad(){
    Widget* background = new Widget("background");
        background->layout.SetOffsetMode(Relative,Relative);
        background->layout.SetOffset(0,0);
        background->layout.SetSizeMode(Relative,Relative);
        background->layout.SetSize(1,1);
        background->components.Add(new SpriteComponent("low_effort_banner"));
        AddWidget(background);
    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    Widget* status_text_widget = new Widget("status_text");
        status_text_widget->layout.SetSize(1024,64);
        h_status_text = new TextComponent("Loading...",loading_font);
        status_text_widget->components.Add(h_status_text);
        status_text_widget->layout.MoveTo(&layout,Center_V,Center_H,{0,4});
    AddWidget(status_text_widget);
}

void LoadingMenu::OnUpdate(int frames){}

void LoadingMenu::SetStatusMessage(wchar* status){
    h_status_text->SetString(status);
}