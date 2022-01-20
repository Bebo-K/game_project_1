#include "main_menu.h"

#include "../widget/standard_widgets.h"

#include "../../log.h"
#include "../gui.h"
#include "../../game.h"

using namespace UI;

void LaunchSingleplayerButtonCallback(){
    logger::debug("Launching server!");
    LoadingMenu* loading_menu = GUI::GetGUI()->loading_menu;
    loading_menu->Open();
    loading_menu->SetStatusMessage(wstr::new_copy(L"Launching local server..."));
    Game::StartLocalServer();
}

void LaunchMultiplayerButtonCallback(){
    logger::debug("Launching mp server!");

}

void GoToOptionsCallback(){
    GUI* ui = GUI::GetGUI();
    ui->options_menu->Open();
    ui->main_menu->Close();
}

void ExitGameCallback(){
    Client::GetClient()->Quit();
}
              
MainMenu::MainMenu(Layout* parent) : Menu(parent){id = Menu::MAIN;}

void MainMenu::OnLoad(){
    Widget* background = new Widget("background");
    background->layout.SetOffsetMode(Relative,Relative);
    background->layout.SetOffset(0,0);
    background->layout.SetSizeMode(Relative,Relative);
    background->layout.SetSize(1,1);
    background->sprites.Resize(1);
        background->sprites.Set(0,new Sprite("low_effort_banner"));
    AddWidget(background);
    
    Widget* play_sp_button = new SimpleButton("play_sp_button",L"Play Singleplayer",312,128,{0.3,0.15,1.0,0.8},LaunchSingleplayerButtonCallback);
    play_sp_button->layout.MoveTo(&layout,Top,Center_H,{0,4});
    
    play_sp_button->layout.SetSizeMode(Relative,Relative);

    Widget* play_mp_button = new SimpleButton("play_mp_button",L"Play Multiplayer",312,128,{0.3,0.15,1.0,0.8},LaunchMultiplayerButtonCallback);
    play_mp_button->layout.MoveTo(&play_sp_button->layout,Below,Center_H,{0,10});
    play_mp_button->layout.SetSizeMode(Relative,Relative);

    Widget* options_button = new SimpleButton("options_button",L"Options",312,128,{0.3,0.15,1.0,0.8},GoToOptionsCallback);
    options_button->layout.MoveTo(&play_mp_button->layout,Below,Center_H,{0,10});
    options_button->layout.SetSizeMode(Relative,Relative);

    Widget* exit_button = new SimpleButton("exit_button",L"Exit",312,128,{0.3,0.15,1.0,0.8},ExitGameCallback);
    exit_button->layout.MoveTo(&options_button->layout,Below,Center_H,{0,10});
    exit_button->layout.SetSizeMode(Relative,Relative);

    
    play_sp_button->selectable->next_down = play_mp_button;
    play_mp_button->selectable->next_up = play_sp_button;
    play_mp_button->selectable->next_down = options_button;
    options_button->selectable->next_up = play_mp_button;
    options_button->selectable->next_down = exit_button;
    exit_button->selectable->next_up = options_button;

    AddWidget(play_sp_button);
    AddWidget(play_mp_button);
    AddWidget(options_button);
    AddWidget(exit_button);
    
    SetSelected(play_sp_button);
}

void MainMenu::OnUnload(){

}
void  MainMenu::OnUpdate(int frames){};
void  MainMenu::OnPaint(){}

bool  MainMenu::OnInput(Input::Event event_type){return false;}
void  MainMenu::OnResize(){}
bool  MainMenu::OnSignal(EventSignal signal){return false;}