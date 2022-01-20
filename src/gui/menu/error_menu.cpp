#include "error_menu.h"
#include "../widget/standard_widgets.h"
#include "../gui.h"

using namespace UI;

void BackToMainMenuCallback(){
    GUI* ui = GUI::GetGUI();
    ui->main_menu->Open();
    ui->error_menu->Close();
}

ErrorMenu::ErrorMenu(Layout* parent) : Menu(parent){
    id = Menu::ERROR_MENU;
    status_text_widget = nullptr;
}

void ErrorMenu::OnLoad(){
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
        status_text_widget->texts.Set(0,new UI_Text(L"Unknown error, uh oh!",loading_font));
    status_text_widget->layout.MoveTo(&layout,Center_V,Center_H,{0,4});
    AddWidget(status_text_widget);

    Widget* main_menu_button = new SimpleButton("main_menu_button",L"Back to main menu",312,128,{0.3,0.15,1.0,0.8},BackToMainMenuCallback);
    main_menu_button->layout.MoveTo(&layout,Bottom,Center_H,{0,4});
    main_menu_button->layout.SetSizeMode(Relative,Relative);
    AddWidget(main_menu_button);
    SetSelected(main_menu_button);
}

void ErrorMenu::OnUpdate(int frames){}

void ErrorMenu::SetStatusMessage(wchar* status){
    status_text_widget->texts[0]->SetString(status);
}