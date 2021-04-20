#include "main_menu.h"


MainMenu::MainMenu(){
    background_img = new Sprite(TextureManager::Get("dat/ui/low_effort_banner.png"));



}


void  MainMenu::OnOpen(){}
void  MainMenu::OnClose(){};
void  MainMenu::OnUpdate(int frames){


};
void  MainMenu::OnPaint(){
    //background_img->width = layout.W;
    //background_img->height = layout.H;
    background_img->Draw();

}
bool  MainMenu::OnInput(Input::EventID event_type){return false;}
void  MainMenu::OnResize(){}
bool  MainMenu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}