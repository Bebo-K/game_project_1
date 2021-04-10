#include "window.h"
#include "../os.h"


UIWindow::UIWindow(){
    type_id=0;
    visible=false;
    active=false;
    layout.X=0;
    layout.Y=0;
    layout.H=Window::height;
    layout.W=Window::width;
}

UIWindow::UIWindow(UIWindow* parent){
    layout.offset.parent = &parent->layout;
    layout.Resize();
    visible=false;
    active=false;
}

void UIWindow::Clear(){
    visible=false;
    active=false;
    for(UIWindow* w:sub_windows){w->Clear();}
    widgets.Clear();
}

Widget* UIWindow::AddWidget(Widget* w,char* name){
    w->layout.offset.parent = &layout;
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* UIWindow::AddWidget(Widget* w,char* name,LayoutOffset pos){
    w->layout.offset = pos;
    w->Resize();
    widgets.Add(name,(byte*)w);
    return w;
}
Widget* UIWindow::GetWidget(char* name){return (Widget*)widgets.Get(name);}
Widget* UIWindow::RemoveWidget(char* name){return (Widget*)widgets.Remove(name);}

void UIWindow::Open(){
    active=true;
    visible=true;
    OnOpen();
}

void UIWindow::Close(){
    active=false;
    OnClose();
}

void UIWindow::Update(int frames){
    if(!active)return;
    OnUpdate(frames);
    for(int i=0;i<widgets.Max();i++){
        Widget* w =(Widget*)widgets.At(i);
        if(w)w->OnUpdate(frames);
    }
    for(UIWindow* w:sub_windows){w->Update(frames);}
}
void UIWindow::Paint(){
    if(!visible)return;
    OnPaint();
    for(int i=0;i<widgets.Max();i++){
        Widget* w =(Widget*)widgets.At(i);
        if(w)w->Paint();
    }
    for(UIWindow* w:sub_windows){w->Paint();}
}

void UIWindow::HandleResize(){
    layout.Resize();
    OnResize();
    for(UIWindow* w:sub_windows){w->HandleResize();}
    for(int i=0;i<widgets.Max();i++){
        Widget* w =(Widget*)widgets.At(i);
        if(w)w->OnResize();
    }
    
}
bool UIWindow::HandleInput(Input::EventID event_type){
    if(!active)return false;
    if(OnInput(event_type))return true;
    for(int i=0;i<widgets.Max();i++){
        Widget* w =(Widget*)widgets.At(i);
        if(w && w->OnInput(event_type))return true;
    }
    for(UIWindow* w:sub_windows){if(w->HandleInput(event_type))return true;}
    return false;
}
bool UIWindow::HandleSignal(int signal_id,int metadata_len, byte* metadata){
    if(!active)return false;
    if(OnSignal(signal_id,metadata_len,metadata))return true;
    for(int i=0;i<widgets.Max();i++){
        Widget* w =(Widget*)widgets.At(i);
        if(w && w->OnSignal(signal_id,metadata_len,metadata))return true;
    }
    for(UIWindow* w:sub_windows){if(w->HandleSignal(signal_id,metadata_len,metadata))return true;}
    return false;
}