#include "main_menu.h"

#include "../widget/button_widget.h"
#include "../widget/text_widget.h"
#include "../widget/shape_widget.h"
#include "../widget/sprite_widget.h"

#include "../../gfx/font_manager.h"
#include "../../game.h"

#include "../../log.h"

void LaunchSingleplayerButtonCallback(){
    logger::warn("Launching server!");
    Game::client->ui.CloseWindow();
    Game::client->LoadScene(0);
    Game::client->SpawnPlayer(Game::client->scene.level.entrances[0]);
}

void LaunchMultiplayerButtonCallback(){
    logger::warn("Launching mp server!");

}
   
              
MainMenu::MainMenu(LayoutRect* parent) : Menu(parent){
    background_img = new Sprite(TextureManager::Get("dat/ui/low_effort_banner.png"));

    FontID Big_Button_Font = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",32);

    ButtonWidget* play_sp_button = new ButtonWidget(LaunchSingleplayerButtonCallback);
        play_sp_button->layout.offset.vertical_mode= Layout::V_TOP;
        play_sp_button->layout.offset.y = -32;
        play_sp_button->layout.W = 312;
        play_sp_button->layout.H = 128;

        RectWidget* play_sp_button_background = new RectWidget({0.3,0.15,1.0,0.8});
        //Texture button_tex = TextureManager::Get("dat/ui/playbutton.png");
        //SpriteWidget* play_button_background = new SpriteWidget(button_tex);
            play_sp_button_background->ParentTo(play_sp_button,"background");

        TextWidget* play_sp_button_text = new TextWidget("Play Singleplayer",Big_Button_Font);
            play_sp_button_text->ParentTo(play_sp_button,"text");

    ButtonWidget* play_mp_button = new ButtonWidget(LaunchMultiplayerButtonCallback);;
        play_mp_button->layout.offset.vertical_mode= Layout::V_BELOW;
        play_mp_button->layout.offset.y = -32;
        play_mp_button->layout.W = 312;
        play_mp_button->layout.H = 128;


        RectWidget* play_mp_button_background = new RectWidget({0.3,0.15,1.0,0.8});
        //Texture button_tex = TextureManager::Get("dat/ui/playbutton.png");
        //SpriteWidget* play_button_background = new SpriteWidget(button_tex);
            play_mp_button_background->ParentTo(play_mp_button,"background");

        TextWidget* play_mp_button_text = new TextWidget("Play Multiplayer",Big_Button_Font);
            play_mp_button_text->ParentTo(play_mp_button,"text");
        
    play_mp_button->ParentTo(play_sp_button,"othabutton");
    widgets.Add(play_sp_button,"play_sp_button");
    //widgets.Add(play_mp_button,"play_mp_button");
}

void  MainMenu::OnOpen(){}
void  MainMenu::OnClose(){};
void  MainMenu::OnUpdate(int frames){


};
void  MainMenu::OnPaint(){
    background_img->Draw();
}
bool  MainMenu::OnInput(Input::Event event_type){return false;}
void  MainMenu::OnResize(){
    background_img->scale.x = ((float)layout.W)/background_img->width;
    background_img->scale.y = ((float)layout.H)/background_img->height;
}
bool  MainMenu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}