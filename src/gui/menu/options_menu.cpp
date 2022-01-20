#include "options_menu.h"

#include "../widget.h"
#include "../widget/standard_widgets.h"
#include "../gui.h"


using namespace UI;

void LiterallyDoNothingCallback(){
}


void ReturnToMainMenuCallback(){
    GUI* ui = GUI::GetGUI();
    ui->options_menu->Close();
    ui->main_menu->Open();
}

OptionsMenu::OptionsMenu(Layout* parent) : Menu(parent){id = Menu::OPTIONS;}

void OptionsMenu::OnLoad(){
    Widget* background = new Widget("background");
    background->layout.relative={0,0,1,1};
    background->layout.width_scale=Relative;
    background->layout.height_scale=Relative;
    background->sprites.Resize(1);
        background->sprites.Set(0,new Sprite("low_effort_banner"));
    AddWidget(background);
    
    Widget* mute_button = new SimpleButton("mute_button",L"Mute",312,128,{0.3,0.15,1.0,0.8},LiterallyDoNothingCallback);
    mute_button->layout.MoveTo(&layout,Top,Center_H,{0,0});

    Widget* back_button = new SimpleButton("back_button",L"Back",312,128,{0.3,0.15,1.0,0.8},ReturnToMainMenuCallback);
    back_button->layout.MoveTo(&layout,Bottom,Center_H,{0,0});

    mute_button->selectable->next_down = back_button;
    back_button->selectable->next_up = mute_button;

    AddWidget(mute_button);
    AddWidget(back_button);
    
    SetSelected(mute_button);


}