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

GUI* GUI::instance = nullptr;

using namespace UI;

GUI::GUI(): 
menus(),
fullscreen_layout(),
debug_widgets(){
    GUI::instance=this;
    logger::info("loading ui...\n");
    Widget* developer_console = new DeveloperConsole();
        developer_console->active=false;
        developer_console->visible=false;
    debug_widgets.Add(developer_console);
    DeveloperConsole::Write("Hello World! GUI Initialized.");

    OnResize(Window::width,Window::height);

    Load();
    main_menu->Open();
}
void GUI::Load(){
    main_menu = new MainMenu(&fullscreen_layout);
    options_menu = new OptionsMenu(&fullscreen_layout);
    loading_menu = new LoadingMenu(&fullscreen_layout);
    ingame_menu = new IngameMenu(&fullscreen_layout);
    menus.Add(main_menu);
    menus.Add(main_menu);
    menus.Add(main_menu);
    menus.Add(main_menu);
}

void GUI::Unload(){
    menus.Clear();
}
void GUI::Reload(){
    Unload();
    Load();
}

GUI* GUI::GetGUI(){return instance;}

Menu* GUI::GetMenu(int id){
    for(Menu* m: menus){if(id == m->id)return m;}
    return null;
}
    
void GUI::Paint(){
    for(Menu* m: menus){m->Paint();}
    for(Widget* w: debug_widgets){w->Paint();}
}
void GUI::Update(Scene* scene, int frames){
    for(Menu* m: menus){m->Update(frames);}
    for(Widget* w: debug_widgets){w->Update(frames);}
}
bool GUI::OnInput(Input::Event event_type){
    for(Widget* w: debug_widgets){if(w->HandleInput(event_type))return true;}
    for(Menu* m: menus){if(m->HandleInput(event_type))return true;}
    return false;
}
void GUI::OnSignal(UI::Signal signal){
    for(Widget* w: debug_widgets){if(w->HandleSignal(signal))return;}
    for(Menu* m: menus){if(m->HandleSignal(signal))return;}
}
void GUI::OnResize(int screen_w,int screen_h){
    for(Menu* m: menus){m->HandleResize();}
    for(Widget* w: debug_widgets){w->HandleResize();}
}


/////////////////////////////OLD
GUI::GUI():{
    current_screen = nullptr;
}

DeveloperConsole* developer_console;

void GUI::Load(): 
main_menu(&fullscreen_layout),
options_menu(&fullscreen_layout),
loading_menu(&fullscreen_layout),
ingame_menu(&fullscreen_layout)
{

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
void GUI::OnSignal(UISignal signal){
    bool handled = false;
    for(Widget* w: debug_widgets){if(w->HandleSignal(signal)){handled=true;};}

    if(!handled && current_screen && current_screen->active){
        handled = current_screen->HandleSignal(signal);
    }
}

void GUI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.absolute = {0,0,screen_w,screen_h};
    
    for(Widget* w: debug_widgets){w->HandleResize();}

    if(current_screen){
        current_screen->HandleResize();
    }
}
