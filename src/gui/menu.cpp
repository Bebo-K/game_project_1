#include "menu.h"
#include "../os.h"

using namespace UI;

Menu::Menu(Layout* parent):layout(parent), widgets(){
    id=0;
    layout.width_scale=Relative;
    layout.height_scale=Relative;
    active=false;
    visible=false;
    OnLoad();
}
Menu::~Menu(){
    OnUnload();
}

void Menu::Open(){
    active=true;
    visible=true;
    OnOpen();
}
void Menu::Close(){
    active=false;
    visible=false;
    OnClose();
}
void Menu::Update(int frames){
    if(!active)return;
    OnUpdate(frames);
    for(Widget* w: widgets){w->Update(frames);}
}
void Menu::Paint(){
    if(!visible)return;
    OnPaint();
    for(Widget* w: widgets){w->Paint();}
}
bool Menu::HandleInput(Input::Event event_type){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleInput(event_type))return true;}
    if(OnInput(event_type))return true;
    return false;
}
void Menu::HandleResize(){
    layout.Resize();
    OnResize();
    for(Widget* w: widgets){w->HandleResize();}
}
bool Menu::HandleSignal(Signal signal){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleSignal(signal))return true;}
    if(OnSignal(signal))return true;
    return false;
}

void Menu::OnOpen(){}
void Menu::OnClose(){}
void Menu::OnUpdate(int frames){}
void Menu::OnPaint(){}
bool Menu::OnInput(Input::Event event_type){return false;}
void Menu::OnResize(){}
bool Menu::OnSignal(Signal signal){return false;}

void Menu::AddWidget(Widget* w){
    w->layout.SetParent(&layout);
    widgets.Add(w);
}
void Menu::RemoveWidget(Widget* w){
    widgets.Delete(w);
}