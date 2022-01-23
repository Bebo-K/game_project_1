#include "loading_menu.h"
#include "../widget/standard_widgets.h"

using namespace UI;

LoadingMenu::LoadingMenu(): Menu(){
    id = Menu::LOADING;
    status_text_widget = nullptr;
}

void LoadingMenu::OnLoad(){
    AddWidget(new MenuBackground());

    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    status_text_widget = new Widget("status_text");
    status_text_widget->layout.width = 1024;
    status_text_widget->layout.height = 64;
    status_text_widget->texts.Resize(1);
        status_text_widget->texts.Set(0,new UI_Text(L"Loading...",loading_font));
    status_text_widget->MoveTo(nullptr,Center_V,Center_H,{0,4});
    AddWidget(status_text_widget);
}

void LoadingMenu::OnUpdate(int frames){}

void LoadingMenu::SetStatusMessage(wchar* status){
    status_text_widget->texts[0]->SetString(status);
    status_text_widget->MoveTo(nullptr,Center_V,Center_H,{0,4});
}