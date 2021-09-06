#include "gui.h"
#include "../os.h"
#include "../config.h"
#include "../log.h"
#include <math.h>
#include "../test/perf.h"


#include "menu.h"
#include "menu/main_menu.h"
#include "menu/loading_menu.h"
#include "menu/ingame_menu.h"

#include "widget/dev_console.h"

GUI::GUI(){
    for(int i=0;i< MENU_STACK_MAX;i++){
        menu_stack[i]=nullptr;
    }
}
void GUI::Load(){
    

}
void GUI::Unload(){
    for(int i=0;i<MENU_STACK_MAX;i++){
        if(menu_stack[i]!= nullptr){
            delete menu_stack[i];
        }
    }
}

void GUI::OpenMenu(MenuID menu){

}
//void OpenMenu(MenuID menu,MenuContext* context);
void GUI::CloseMenu(MenuID menu){

}

bool GUI::IsMenuOpen(MenuID menu){
    for(int i=0;i<MENU_STACK_MAX;i++){
        if(menu_stack[i]->id == menu){
           return true;
        }
    } 
    return false;
}

template <class T> T* GUI::GetMenu(MenuID menu){
    Menu* ret = nullptr;
    for(int i=0;i<MENU_STACK_MAX;i++){
        if(menu_stack[i]->id == menu){
            return (T*)menu_stack[i];
        }
    }
    return (T*)null;
}

//For modding later:
//void RegisterMenu(MenuID menu,Menu* menu);

//Client events
void GUI::Paint();
void GUI::Update(Scene* scene, int frames);

//UI interaction events
bool GUI::OnInput(Input::Event event_type);
void GUI::OnSignal(int signal_id,int metadata_len, byte* metadata);
void GUI::OnResize(int screen_w,int screen_h);


GUI::GUI():debug_widgets(&this->fullscreen_layout){
    current_screen = nullptr;
}

DeveloperConsole* developer_console;

void GUI::Load(){
    logger::info("loading ui...\n");
    developer_console = new DeveloperConsole();
    developer_console->active=false;
    DeveloperConsole::Write("hello world!");
    OnResize(Window::width,Window::height);
}
    
    
UIWindow* GUI::BuildMenu(int window_id){
    switch(window_id){
        case UI::MAIN_MENU: return new MainMenu(&this->fullscreen_layout); break;
        case UI::LOADING: return new LoadingMenu(&this->fullscreen_layout); break;
        case UI::INGAME: return new IngameMenu(&this->fullscreen_layout); break;
        default: break;
    }
    return nullptr;
}

UIWindow* GUI::OpenWindow(int window_id){
    if(current_screen != nullptr){CloseWindow();}
    current_screen = BuildMenu(window_id);
    current_screen->active=true;
    current_screen->visible=true;
    current_screen->layout.offset.parent=&fullscreen_layout;
    return current_screen;
}

void GUI::CloseWindow(){
    free(this->current_screen);
    current_screen=nullptr;
}

void GUI::Paint(){
    if(current_screen && current_screen->visible){
        current_screen->Paint();
    }
    for(Widget* w: debug_widgets){w->Paint();}
}

void GUI::Update(Scene* scene, int frames){
    if(current_screen && current_screen->active){
        current_screen->Update(frames);
    }
 
    for(Widget* w: debug_widgets){w->Update(frames);}
}

void GUI::Unload(){
    logger::info("unloading ui...\n");
    delete developer_console;
}

bool GUI::OnInput(Input::Event event_type){
    bool handled = false;
    
    for(Widget* w: debug_widgets){if(w->HandleInput(event_type)){handled=true;};}

    if(current_screen && current_screen->active){
        handled = current_screen->HandleInput(event_type);
    }
    return handled;
}
void GUI::OnSignal(int signal_id,int metadata_len, byte* metadata){
    bool handled = false;
    for(Widget* w: debug_widgets){if(w->HandleSignal(signal_id,metadata_len,metadata)){handled=true;};}

    if(!handled && current_screen && current_screen->active){
        handled = current_screen->HandleSignal(signal_id,metadata_len,metadata);
    }
}

void GUI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.absolute = {0,0,screen_w,screen_h};
    
    for(Widget* w: debug_widgets){w->OnResize();}

    if(current_screen){
        current_screen->HandleResize();
    }
}
