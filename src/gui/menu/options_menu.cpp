#include "options_menu.h"

#include "../widget.h"
#include "../widget/simple_button.h"
#include "../widget/sprite_component.h"
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
    background->components.Add(new SpriteComponent("dat/ui/low_effort_banner.png"));
    AddWidget(background);
    
    Widget* play_sp_button = BuildSimpleButton("options_button","Yes?",312,128,{0.3,0.15,1.0,0.8},LiterallyDoNothingCallback);
    play_sp_button->layout.MoveTo(&layout,Top,Center_H,{0,0});
    AddWidget(play_sp_button);

    Widget* back_button = BuildSimpleButton("back_button","Back",312,128,{0.3,0.15,1.0,0.8},ReturnToMainMenuCallback);
    back_button->layout.MoveTo(&layout,Bottom,Center_H,{0,0});
    AddWidget(back_button);
}