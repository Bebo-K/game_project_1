#include "window.h"
#include "../os.h"


UI::Menu::Menu():layout(){
    active=false;
    visible=false;
    id=0;
}
void UI::Menu::Open(Layout* menu_area){
    layout.SetParent(menu_area);
    active=true;
    visible=true;
    OnOpen();
}
void UI::Menu::Close(){
    active=false;
    visible=false;
    OnClose();
}
void UI::Menu::Update(int frames){OnUpdate(frames);}
void UI::Menu::Paint(){OnPaint();}
bool UI::Menu::HandleInput(Input::Event event_type){return OnInput(event_type);}
void UI::Menu::HandleResize(){layout.Resize();OnResize();}
bool UI::Menu::HandleSignal(int signal_id,int metadata_len, byte* metadata){return OnSignal(signal_id,metadata_len,metadata);}

void UI::Menu::OnOpen(){}
void UI::Menu::OnClose(){}
void UI::Menu::OnUpdate(int frames){}
void UI::Menu::OnPaint(){}
bool UI::Menu::OnInput(Input::Event event_type){return false;}
void UI::Menu::OnResize(){}
bool UI::Menu::OnSignal(int signal_id,int metadata_len, byte* metadata){return false;}

/*

UIWindow::UIWindow():widgets(&this->layout){
    type_id=0;
    visible=false;
    active=false;
    layout.h_pos_mode = Layout::MATCH_PARENT;
    layout.v_pos_mode = Layout::MATCH_PARENT;
    layout.h_fill_mode = Layout::MATCH_PARENT;
    layout.v_fill_mode = Layout::MATCH_PARENT;
}

UIWindow::UIWindow(UIWindow* parent):widgets(&this->layout){
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

void UIWindow::Open(){
    active=true;
    visible=true;
    OnOpen();
    for(Widget* w : widgets){w->Destroy();}
    for(UIWindow* w:sub_windows){w->Close();}
}

void UIWindow::Close(){
    active=false;
    OnClose();
    for(Widget* w : widgets){w->Destroy();}
    for(UIWindow* w:sub_windows){w->Close();}
}

void UIWindow::Update(int frames){
    if(!active)return;
    OnUpdate(frames);
    for(Widget* w : widgets){w->Update(frames);}
    for(UIWindow* w:sub_windows){w->Update(frames);}
}
void UIWindow::Paint(){
    if(!visible)return;
    OnPaint();
    for(Widget* w : widgets){w->Paint();}
    for(UIWindow* w:sub_windows){w->Paint();}
}

void UIWindow::HandleResize(){
    layout.Resize();
    OnResize();
    for(UIWindow* w:sub_windows){w->HandleResize();}
    for(Widget* w : widgets){w->HandleResize();}
}

bool UIWindow::HandleInput(Input::Event event_type){
    //if(!active)return false; Windows should keep input handlers on to listen for activation key. (e.g. 'M' for map)
    if(OnInput(event_type))return true;
    for(Widget* w : widgets){if(w->HandleInput(event_type))return true;}
    for(UIWindow* w:sub_windows){if(w->HandleInput(event_type))return true;}
    return false;
}
bool UIWindow::HandleSignal(int signal_id,int metadata_len, byte* metadata){
    //if(!active)return false; Windows should keep signal handlers on to listen for activation events.
    if(OnSignal(signal_id,metadata_len,metadata))return true;
    for(Widget* w : widgets){if(w->HandleSignal(signal_id,metadata_len,metadata))return true;}
    for(UIWindow* w:sub_windows){if(w->HandleSignal(signal_id,metadata_len,metadata))return true;}
    return false;
}

*/