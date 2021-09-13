#include "main_menu.h"

#include "widget/simple_button.h"
#include "widget/sprite_component.h"

#include "../../log.h"
#include "../../client/client.h"
#include "../gui.h"


using namespace UI;

void LaunchSingleplayerButtonCallback(){
    logger::warn("Launching server!");
    GUI::GetGUI()->loading_menu->Open();
    GUI::GetGUI()->main_menu->Close();

    /*
    Game::client->StartLoadScene(1);
    Game::client->SpawnPlayer(Game::client->scene.level.entrances[0]);

    Game::client->ui.OpenMenu(MenuType::INGAME);
    Game::client->ui.CloseMenu(MenuType::LOADING);
    */
}

void LaunchMultiplayerButtonCallback(){
    logger::warn("Launching mp server!");

}

void ExitGameCallback(){
    Client::GetClient()->Quit();
}
              
MainMenu::MainMenu(Layout* parent) : Menu(parent){id = Menu::MAIN;}

void MainMenu::OnLoad(){
    Widget* background = new Widget("background");
    background->layout.relative={0,0,1,1};
    background->layout.width_scale=Relative;
    background->layout.height_scale=Relative;
    background->components.Add(new SpriteComponent("dat/ui/low_effort_banner.png"));
    AddWidget(background);
    
    Widget* play_sp_button = BuildSimpleButton("play_sp_button","Play Singleplayer",312,128,{0.3,0.15,1.0,0.8},LaunchSingleplayerButtonCallback);
    play_sp_button->layout.MoveTo(&layout,Top,Center_H,{0,0});
    AddWidget(play_sp_button);

    Widget* play_mp_button = BuildSimpleButton("play_mp_button","Play Multiplayer",312,128,{0.3,0.15,1.0,0.8},LaunchMultiplayerButtonCallback);
    play_mp_button->layout.MoveTo(&play_sp_button->layout,Below,Center_H,{0,10});
    AddWidget(play_mp_button);
}

void MainMenu::OnUnload(){

}

void  MainMenu::OnOpen(){}
void  MainMenu::OnClose(){};
void  MainMenu::OnUpdate(int frames){};
void  MainMenu::OnPaint(){}

bool  MainMenu::OnInput(Input::Event event_type){return false;}
void  MainMenu::OnResize(){}
bool  MainMenu::OnSignal(Signal signal){return false;}