#include <game_project_1/gui/panel.hpp>


using namespace UI;

Panel::Panel(int w,int h):layout(w,h), widgets(){
    active=false;
    visible=false;
}
Panel::~Panel(){}

void Panel::Show(){
    active=true;
    visible=true;
}
void Panel::Hide(){
    active=false;
    visible=false;
}
void Panel::Update(Timestep delta){
    if(!active)return;
    for(Widget* w: widgets){w->Update(delta);}
}
void Panel::Paint(){
    if(!visible)return;
    for(Widget* w: widgets){w->Paint();}
}
bool Panel::OnInput(Input::Event event_type){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleInput(event_type))return true;}
    return false;
}
bool Panel::OnSignal(EventSignal signal){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleSignal(signal))return true;}
    return false;
}
void Panel::AddWidget(Widget* w){
    widgets.Add(w);
}
void Panel::RemoveWidget(Widget* w){
    widgets.Remove(w);
}