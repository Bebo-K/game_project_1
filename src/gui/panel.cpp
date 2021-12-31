#include "panel.h"


using namespace UI;

Panel::Panel(Layout* parent):layout(parent), widgets(){
    layout.width_scale=Relative;
    layout.height_scale=Relative;
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
void Panel::Update(int frames){
    if(!active)return;
    for(Widget* w: widgets){w->Update(frames);}
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
void Panel::OnResize(){
    layout.Resize();
    for(Widget* w: widgets){w->HandleResize();}
}
bool Panel::OnSignal(EventSignal signal){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleSignal(signal))return true;}
    return false;
}

void Panel::AddWidget(Widget* w){
    w->layout.SetParent(&layout);
    widgets.Add(w);
}
void Panel::RemoveWidget(Widget* w){
    widgets.Remove(w);
}