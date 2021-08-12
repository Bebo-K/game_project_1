#include "ui.h"
#include "../os.h"
#include "../config.h"
#include "../log.h"
#include <math.h>
#include "../test/perf.h"

#include "menu/main_menu.h"
#include "menu/loading_menu.h"
#include "menu/ingame_menu.h"

#include "widget/dev_console.h"

UI::UI():debug_widgets(&this->fullscreen_layout){
    current_screen = nullptr;
}

DeveloperConsole* developer_console;

void UI::Load(){
    logger::info("loading ui...\n");
    developer_console = new DeveloperConsole();
    developer_console->active=false;
    DeveloperConsole::Write("hello world!");
    OnResize(Window::width,Window::height);
}
    
    
UIWindow* UI::BuildMenu(int window_id){
    switch(window_id){
        case UI::MAIN_MENU: return new MainMenu(&this->fullscreen_layout); break;
        case UI::LOADING: return new LoadingMenu(&this->fullscreen_layout); break;
        case UI::INGAME: return new IngameMenu(&this->fullscreen_layout); break;
        default: break;
    }
    return nullptr;
}

UIWindow* UI::OpenWindow(int window_id){
    if(current_screen != nullptr){CloseWindow();}
    current_screen = BuildMenu(window_id);
    current_screen->active=true;
    current_screen->visible=true;
    current_screen->layout.offset.parent=&fullscreen_layout;
    return current_screen;
}

void UI::CloseWindow(){
    free(this->current_screen);
    current_screen=nullptr;
}

void UI::Paint(){
    if(current_screen && current_screen->visible){
        current_screen->Paint();
    }
    for(Widget* w: debug_widgets){w->Paint();}
}

void UI::Update(Scene* scene, int frames){
    if(current_screen && current_screen->active){
        current_screen->Update(frames);
    }
 
    for(Widget* w: debug_widgets){w->Update(frames);}
}

void UI::Unload(){
    logger::info("unloading ui...\n");
    delete developer_console;
}

bool  UI::OnInput(Input::Event event_type){
    bool handled = false;
    
    for(Widget* w: debug_widgets){if(w->HandleInput(event_type)){handled=true;};}

    if(current_screen && current_screen->active){
        handled = current_screen->HandleInput(event_type);
    }
    return handled;
}
void UI::OnSignal(int signal_id,int metadata_len, byte* metadata){
    bool handled = false;
    for(Widget* w: debug_widgets){if(w->HandleSignal(signal_id,metadata_len,metadata)){handled=true;};}

    if(!handled && current_screen && current_screen->active){
        handled = current_screen->HandleSignal(signal_id,metadata_len,metadata);
    }
}

void UI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.X=0;
    fullscreen_layout.Y=0;
    fullscreen_layout.W=screen_w;
    fullscreen_layout.H=screen_h;
    
    for(Widget* w: debug_widgets){w->OnResize();}

    if(current_screen){
        current_screen->HandleResize();
    }
}
