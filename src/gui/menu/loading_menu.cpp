#include "loading_menu.h"

using namespace UI;

LoadingMenu::LoadingMenu(Layout* parent) : Menu(parent){
    id = Menu::LOADING;
    status_text_widget = nullptr;
}

void LoadingMenu::OnLoad(){
    Widget* background = new Widget("background");
    background->layout.SetOffsetMode(Relative,Relative);
    background->layout.SetOffset(0,0);
    background->layout.SetSizeMode(Relative,Relative);
    background->layout.SetSize(1,1);
    background->sprites.Resize(1);
        background->sprites.Set(0,new Sprite("low_effort_banner"));
    AddWidget(background);

    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    status_text_widget = new Widget("status_text");
    status_text_widget->layout.SetSize(1024,64);
    status_text_widget->texts.Resize(1);
        status_text_widget->texts.Set(0,new UI_Text(L"Loading...",loading_font));
    status_text_widget->layout.MoveTo(&layout,Center_V,Center_H,{0,4});
    AddWidget(status_text_widget);
}

void LoadingMenu::OnUpdate(int frames){}

void LoadingMenu::SetStatusMessage(wchar* status){
    status_text_widget->texts[0]->SetString(status);
}