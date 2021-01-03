#include "menu.h"

//TODO: unstub

void Menu::SetLayout(int height,int width,LayoutOffset pos){
    layout.offset=pos;
    layout.H = height;
    layout.W = width;
    layout.Resize();
}
Widget* Menu::AddWidget(Widget* w,char* name){
    w->layout.offset = LayoutOffset(&layout);
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* Menu::AddWidget(Widget* w,char* name,LayoutOffset pos){
    w->layout.offset = pos;
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* Menu::GetWidget(char* name){return (Widget*)widgets.Get(name);}
Widget* Menu::RemoveWidget(char* name){return (Widget*)widgets.Remove(name);}

void Menu::Update(int frames){}
void Menu::Paint(){}

bool Menu::HandleInput(InputCode event_type){return false;}
void Menu::HandleResize(){}
bool Menu::HandleSignal(int signal_id,int metadata_len, byte* metadata){return false;}
       