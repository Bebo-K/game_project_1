#include <game_project_1/gui/menu/options_menu.hpp>
#include <game_project_1/gui/widget.hpp>
#include <game_project_1/gui/widget/standard_widgets.hpp>
#include <game_project_1/gui/gui.hpp>


using namespace UI;

void LiterallyDoNothingCallback(){
}


void ReturnToMainMenuCallback(){
    GUI* ui = GUI::GetGUI();
    ui->options_menu->Close();
    ui->main_menu->Open();
}

OptionsMenu::OptionsMenu(): Menu(){id = Menu::OPTIONS;}

void OptionsMenu::OnLoad(){
    AddWidget(new MenuBackground());
    
    Widget* mute_button = new SimpleButton("mute_button",L"Mute",312,128,{0.3,0.15,1.0,0.8},LiterallyDoNothingCallback);
    mute_button->MoveTo(nullptr,Top,Center_H,{0,0});

    Widget* back_button = new SimpleButton("back_button",L"Back",312,128,{0.3,0.15,1.0,0.8},ReturnToMainMenuCallback);
    back_button->MoveTo(nullptr,Bottom,Center_H,{0,0});

    mute_button->selectable->next_down = back_button;
    back_button->selectable->next_up = mute_button;

    AddWidget(mute_button);
    AddWidget(back_button);
    
    SetSelected(mute_button);


}