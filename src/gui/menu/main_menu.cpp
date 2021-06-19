#include "main_menu.h"

#include "../widget/button_widget.h"
#include "../widget/text_widget.h"
#include "../widget/shape_widget.h"

#include "../../gfx/font_manager.h"

void LaunchSingleplayerButtonCallback(){


}
              
MainMenu::MainMenu(Layout* parent){
    layout.offset.parent=parent;
    if(parent != null){
        layout.W = parent->W;
        layout.H = parent->H;
    }
    else{
        layout.W = 512;
        layout.H = 256;
    }

    background_img = new Sprite(TextureManager::Get("dat/ui/low_effort_banner.png"));

    FontID Big_Button_Font = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",32);

    //this->AddWidget("play_button");
    ButtonWidget* play_button = new ButtonWidget(LaunchSingleplayerButtonCallback);
        play_button->layout.W = 256;
        play_button->layout.H = 64;

        TextWidget* play_button_text = new TextWidget("Play",Big_Button_Font);
            play_button_text->ParentTo(play_button,"text");
        
        RectWidget* play_button_background = new RectWidget({0.3,0.3,1.0,0.8});
            play_button_background->ParentTo(play_button,"background");

    widgets.Add(play_button,"play_button");
}

void  MainMenu::OnOpen(){}
void  MainMenu::OnClose(){};
void  MainMenu::OnUpdate(int frames){


};
void  MainMenu::OnPaint(){
    background_img->Draw();

}
bool  MainMenu::OnInput(Input::EventID event_type){return false;}
void  MainMenu::OnResize(){
    background_img->scale.x = ((float)layout.W)/background_img->width;
    background_img->scale.y = ((float)layout.H)/background_img->height;
}
bool  MainMenu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}