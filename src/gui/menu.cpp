#include "menu.h"
#include "../os.h"

using namespace UI;

Menu::Menu():widgets(){
    id=0;
    active=false;
    visible=false;
    selected=nullptr;
}
Menu::~Menu(){}

void Menu::Open(){
    active=true;
    visible=true;
    OnOpen();
}
void Menu::Close(){
    if(!active)return;
    active=false;
    visible=false;
    OnClose();
}
void Menu::Load(){
    OnLoad();
}
void Menu::Unload(){
    OnUnload();
    widgets.Clear();
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

bool Menu::HandleSelectionInput(Input::Event event_type){
    if(selected != nullptr){
        if(event_type==Input::Move && !selected->selectable->locked){
            Controller::Axis axis = Controller::GetAxis(Controller::Move);
            if(axis.IsJustUp() && selected->selectable->next_up != nullptr){
                SetSelected(selected->selectable->next_up);
                return true;
            }
            if(axis.IsJustDown() && selected->selectable->next_down != nullptr){
                SetSelected(selected->selectable->next_down);
                return true;
            }
            if(axis.IsJustLeft() && selected->selectable->next_left != nullptr){
                SetSelected(selected->selectable->next_left);
                return true;
            }
            if(axis.IsJustRight() && selected->selectable->next_right != nullptr){
                SetSelected(selected->selectable->next_right);
                return true;
            }
        }
        return selected->selectable->onSelectedInput(selected,event_type);
    }
    return false;
}

bool Menu::HandleClickInput(Input::Event event_type){
    bool handled = false;
    if(event_type== Input::PC_Cursor || event_type== Input::PC_LClick){
        point_i cursor_pos = ScreenToClientSpace(Controller::GetPCCursor());
        Controller::Button cursor_lbutton = Controller::GetPCLeftMouse();
        //Controller::Button cursor_rbutton = Controller::GetPCLeftMouse();
        for(Widget* w: widgets){
            if(w == nullptr || w->clickable == nullptr)continue;
            if(w->layout.Rect().contains(cursor_pos)){
                if(cursor_lbutton.IsDown()){
                    if(cursor_lbutton.IsJustPressed()){
                        w->clickable->onClickEffect(w);
                        w->clickable->state = Pressed;
                        if(w->selectable != nullptr && (selected == nullptr || !selected->selectable->locked)){SetSelected(w);}
                        handled = true;
                    }
                }
                else{
                    if(cursor_lbutton.IsJustReleased()){
                        if(w->clickable->state == Pressed){
                            w->clickable->onClickReleaseEffect(w);
                            w->clickable->onClickAction(w);
                            w->clickable->state = Released;
                            handled = true;
                        }
                    }
                    else{
                        if(w->clickable->state != Hovering){w->clickable->onHoverEffect(w);}
                        w->clickable->state = Hovering;
                    }
                }
            }
            else{
                if(!cursor_lbutton.IsDown() && w->clickable->state == Pressed){w->clickable->onClickReleaseEffect(w);}
                if(w->clickable->state == Hovering){w->clickable->onStopHoverEffect(w);}
                w->clickable->state = Not_Pressed;
            }
        }
    }
    return handled;
}

bool Menu::HandleInput(Input::Event event_type){
    if(!active)return false;
    if(HandleSelectionInput(event_type))return true;
    if(HandleClickInput(event_type))return true;
    for(Widget* w: widgets){if(w->HandleInput(event_type))return true;}
    return OnInput(event_type);
}

void Menu::HandleResize(){
    OnResize();
    for(Widget* w: widgets){w->HandleResize();}
}
bool Menu::HandleSignal(EventSignal signal){
    if(!active)return false;
    for(Widget* w: widgets){if(w->HandleSignal(signal))return true;}
    return OnSignal(signal);
}
void Menu::AddWidget(Widget* w){
    widgets.Add(w);
}
void Menu::RemoveWidget(Widget* w){
    widgets.Remove(w);
    delete w;
}

Widget* Menu::GetWidget(char* name){
    for(Widget* w:widgets){
        if(cstr::compare(w->name,name)){return w;}
    }
    return nullptr;
}

void Menu::SetSelected(Widget* w){
    if(!w->IsSelectable())return;
    if(selected != nullptr){selected->selectable->onStopHighlightEffect(selected);}
    selected = w;
    selected->selectable->onHighlightEffect(w);
}

//default virtual handlers
void Menu::OnOpen(){}
void Menu::OnClose(){}
void Menu::OnLoad(){}
void Menu::OnUnload(){}
void Menu::OnUpdate(int frames){}
void Menu::OnPaint(){}
bool Menu::OnInput(Input::Event event_type){return false;}
void Menu::OnResize(){}
bool Menu::OnSignal(EventSignal signal){return false;}