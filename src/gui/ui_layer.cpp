#include "ui_layer.h"
#include "../os.h"

UILayer::UILayer(){
    active=false;
    screen_rect.X=0;
    screen_rect.Y=0;
    screen_rect.H=Window::height;
    screen_rect.W=Window::width;
}

Widget* UILayer::AddWidget(Widget* w,char* name){
    w->layout.offset.parent = &screen_rect;
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* UILayer::AddWidget(Widget* w,char* name,LayoutOffset pos){
    w->layout.offset = pos;
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* UILayer::GetWidget(char* name){return (Widget*)widgets.Get(name);}
Widget* UILayer::RemoveWidget(char* name){return (Widget*)widgets.Remove(name);}

void UILayer::Update(int frames){
    if(!active)return;
    for(byte* w:widgets){((Widget*)w)->OnUpdate(frames);}
    for(Menu* m:menus){m->Update(frames);}
}
void UILayer::Paint(){
    if(!active)return;
    for(Menu* m:menus){m->Paint();}
    for(int i=0;i<widgets.Count();i++){
        Widget* pWidget = (Widget*)widgets.begin()[i];
        if(pWidget != null){pWidget->Paint();}
    }
    //for(byte* w:widgets){
    //    if(w ==null)continue;
    //    ((Widget*)w)->Paint();}
}
void UILayer::Resize(int screen_w,int screen_h){
    if(!active)return;
    screen_rect.W=screen_w;
    screen_rect.H=screen_h;

    for(Menu* m:menus){m->HandleResize();}
    for(byte* w:widgets){((Widget*)w)->OnResize();}
}
bool UILayer::OnInput(Input::EventID event_type){
    if(!active)return false;
    for(byte* w:widgets){
        if(((Widget*)w)->OnInput(event_type))return true;
        }
    for(Menu* m:menus){if(m->HandleInput(event_type))return true;}
    return false;
}
bool UILayer::OnSignal(int signal_id,int metadata_len, byte* metadata){
    if(!active)return false;
    for(byte* w:widgets){if(((Widget*)w)->OnSignal(signal_id,metadata_len,metadata))return true;}
    for(Menu* m:menus){if(m->HandleSignal(signal_id,metadata_len,metadata))return true;}
    return false;
}