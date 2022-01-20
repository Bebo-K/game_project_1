#include "character_create_menu.h"

#include "../widget/standard_widgets.h"

#include "../gui.h"
#include "../../client/client.h"

using namespace UI;


void ExitServerCallback(){
    Client::GetClient()->Quit();
}
              
CharacterCreateMenu::CharacterCreateMenu(Layout* parent) : Menu(parent){id = Menu::CHARACTER_CREATE;}

void CharacterCreateMenu::OnLoad(){
    Widget* background = new Widget("background");
    background->layout.SetOffsetMode(Relative,Relative);
    background->layout.SetOffset(0,0);
    background->layout.SetSizeMode(Relative,Relative);
    background->layout.SetSize(1,1);
    background->sprites.Resize(1);
        background->sprites.Set(0,new Sprite(TextureManager::Get("low_effort_banner")));
    AddWidget(background);
    
    FontID loading_font = FontManager::LoadFontFace("SourceSansPro-Regular",32);
    Widget* status_text_widget = new Widget("status_text");
    status_text_widget->layout.SetSize(1024,64);
    status_text_widget->texts.Resize(1);
        status_text_widget->texts.Set(0,new UI_Text(L"Build a character:",loading_font));
    status_text_widget->layout.MoveTo(&layout,Top,Center_H,{0,28});
    AddWidget(status_text_widget);
}

void CharacterCreateMenu::OnUnload(){

}
void  CharacterCreateMenu::OnUpdate(int frames){};
void  CharacterCreateMenu::OnPaint(){}

bool  CharacterCreateMenu::OnInput(Input::Event event_type){return false;}
void  CharacterCreateMenu::OnResize(){}
bool  CharacterCreateMenu::OnSignal(EventSignal signal){return false;}