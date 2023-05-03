#include <game_project_1/gui/menu/main_menu.hpp>
#include <game_project_1/gui/widget/standard_widgets.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/gui/gui.hpp>
#include <game_project_1/game.hpp>

using namespace UI;

void LaunchSingleplayerButtonCallback(){
    logger::debug("Launching server!");
    GUI* gui = GUI::GetGUI();
    gui->loading_menu->Open();
    gui->main_menu->Close();
    gui->loading_menu->SetStatusMessage(wstr::new_copy(L"Launching local server..."));
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
              
MainMenu::MainMenu() : Menu(){id = Menu::MAIN;}

void MainMenu::OnLoad(){
    AddWidget(new MenuBackground());
    
    Widget* play_sp_button = new SimpleButton("play_sp_button",L"Play Singleplayer",312,128,{0.3,0.15,1.0,0.8},LaunchSingleplayerButtonCallback);
    play_sp_button->MoveTo(nullptr,Top,Center_H,{0,4});

    Widget* play_mp_button = new SimpleButton("play_mp_button",L"Play Multiplayer",312,128,{0.3,0.15,1.0,0.8},LaunchMultiplayerButtonCallback);
    play_mp_button->MoveTo(play_sp_button,Below,Center_H,{0,10});

    Widget* options_button = new SimpleButton("options_button",L"Options",312,128,{0.3,0.15,1.0,0.8},GoToOptionsCallback);
    options_button->MoveTo(play_mp_button,Below,Center_H,{0,10});

    Widget* exit_button = new SimpleButton("exit_button",L"Exit",312,128,{0.3,0.15,1.0,0.8},ExitGameCallback);
    exit_button->MoveTo(options_button,Below,Center_H,{0,10});

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