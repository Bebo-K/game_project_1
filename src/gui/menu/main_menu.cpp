#include "main_menu.h"

#include "widget/simple_button.h"
#include "../../game.h"

#include "../../log.h"


using namespace UI;

void LaunchSingleplayerButtonCallback(){
    logger::warn("Launching server!");
    Game::client->ui.OpenMenu(MenuType::LOADING);
    Game::client->ui.CloseMenu(MenuType::MAIN_MENU);

    Game::client->StartLoadScene(1);
    Game::client->SpawnPlayer(Game::client->scene.level.entrances[0]);

    Game::client->ui.OpenMenu(MenuType::INGAME);
    Game::client->ui.CloseMenu(MenuType::LOADING);
}

void LaunchMultiplayerButtonCallback(){
    logger::warn("Launching mp server!");

}

void ExitGameCallback(){
    Game::running = false;
}
              
MainMenu::MainMenu() : UI::Menu(){
    id = MenuType::MAIN_MENU;

    Widget* background = new Widget("background");
    background->layout.relative={0,0,1,1};
    background->components.Add(new SpriteComponent("dat/ui/low_effort_banner.png"));
    AddWidget(background);
    
    Widget* play_sp_button = BuildSimpleButton("play_sp_button","Play Singleplayer",312,128,{0.3,0.15,1.0,0.8},LaunchSingleplayerButtonCallback);
    play_sp_button->layout.MoveTo(this->layout,TOP,CENTER_H,{0,0});
    AddWidget(play_sp_button);

    Widget* play_mp_button = BuildSimpleButton("play_mp_button","Play Multiplayer",312,128,{0.3,0.15,1.0,0.8},LaunchMultiplayerButtonCallback);
    play_mp_button->layout.MoveTo(play_sp_button->layout,BELOW,CENTER_H,{0,10});
    AddWidget(play_mp_button);

    
    background_img = new Sprite(TextureManager::Get());

    FontID Big_Button_Font = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",32);

    play_sp_button = new ButtonWidget(LaunchSingleplayerButtonCallback);
        play_sp_button->layout.v_offset_type = UI::TOP;
        play_sp_button->layout.offset.y = -32;
        play_sp_button->layout.absolute.w = 312;
        play_sp_button->layout.absolute.h = 128;
        //play_sp_button->layout.width_scale= UI::RATIO;
        //play_sp_button->layout.height_scale= UI::RATIO;
        
        RectWidget* play_sp_button_background = new RectWidget({0.3,0.15,1.0,0.8});
        //Texture button_tex = TextureManager::Get("dat/ui/playbutton.png");
        //SpriteWidget* play_button_background = new SpriteWidget(button_tex);
            play_sp_button_background->ParentTo(play_sp_button,"background");

        TextWidget* play_sp_button_text = new TextWidget("Play Singleplayer",Big_Button_Font);
            play_sp_button_text->ParentTo(play_sp_button,"text");

    play_mp_button = new ButtonWidget(LaunchMultiplayerButtonCallback);;
        play_mp_button->layout.v_offset_type = UI::BELOW;
        play_mp_button->layout.offset.y = -32;
        play_mp_button->layout.absolute.w = 312;
        play_mp_button->layout.absolute.h = 128;


        RectWidget* play_mp_button_background = new RectWidget({0.3,0.15,1.0,0.8});
        //Texture button_tex = TextureManager::Get("dat/ui/playbutton.png");
        //SpriteWidget* play_button_background = new SpriteWidget(button_tex);
            play_mp_button_background->ParentTo(play_mp_button,"background");

        TextWidget* play_mp_button_text = new TextWidget("Play Multiplayer",Big_Button_Font);
            play_mp_button_text->ParentTo(play_mp_button,"text");
        
    play_mp_button->ParentTo(play_sp_button,"othabutton");
}

void  MainMenu::OnOpen(){}
void  MainMenu::OnClose(){};
void  MainMenu::OnUpdate(int frames){


};
void  MainMenu::OnPaint(){
    background_img->Draw();
    play_sp_button->OnPaint();
    play_mp_button->OnPaint();
}

bool  MainMenu::OnInput(Input::Event event_type){return false;}
void  MainMenu::OnResize(){
    background_img->scale.x = ((float)layout.W)/background_img->width;
    background_img->scale.y = ((float)layout.H)/background_img->height;
}
bool  MainMenu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}