#include <game_project_1/gui/menu/error_menu.hpp>
#include <game_project_1/gui/widget/standard_widgets.hpp>
#include <game_project_1/gui/gui.hpp>

using namespace UI;

void BackToMainMenuCallback(){
    GUI* ui = GUI::GetGUI();
    ui->main_menu->Open();
    ui->error_menu->Close();
}

ErrorMenu::ErrorMenu(): Menu(){
    id = Menu::ERROR_MENU;
    status_text_widget = nullptr;
}

void ErrorMenu::OnLoad(){
    AddWidget(new MenuBackground());
        
    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    status_text_widget = new Widget("status_text");
        status_text_widget->layout.width = 1024;
        status_text_widget->layout.height = 64;
        status_text_widget->texts.Resize(1);
        status_text_widget->texts.Set(0,new DrawableText(L"Unknown error, uh oh!",loading_font));
    status_text_widget->MoveTo(nullptr,Center_V,Center_H,{0,4});
    AddWidget(status_text_widget);

    Widget* main_menu_button = new SimpleButton("main_menu_button",L"Back to main menu",312,128,{0.3,0.15,1.0,0.8},BackToMainMenuCallback);
    main_menu_button->MoveTo(nullptr,Bottom,Center_H,{0,4});
    AddWidget(main_menu_button);
    SetSelected(main_menu_button);
}

void ErrorMenu::OnUpdate(int frames){}

void ErrorMenu::SetStatusMessage(wchar* status){
    status_text_widget->texts[0]->SetString(status);
}