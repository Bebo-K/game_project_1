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

GUI::GUI(): fullscreen_layout(),debug_widgets(),menus(){
    GUI::instance=this;

    main_menu = new MainMenu(&fullscreen_layout);
    options_menu = new OptionsMenu(&fullscreen_layout);
    loading_menu = new LoadingMenu(&fullscreen_layout);
    ingame_menu = new IngameMenu(&fullscreen_layout);
    menus.Add(main_menu);
    menus.Add(options_menu);
    menus.Add(loading_menu);
    menus.Add(ingame_menu);
}

void GUI::Load(){
    logger::info("loading ui...\n");

    main_menu->Load();
    options_menu->Load();
    loading_menu->Load();
    ingame_menu->Load();
    Widget* developer_console = new DeveloperConsole();
    debug_widgets.Add(developer_console);
    
    DeveloperConsole::Write("Hello World! GUI Initialized.");

    OnResize(Window::width,Window::height);
}

void GUI::Unload(){
    for(Menu* m: menus){m->Unload();}
}
void GUI::Reload(){
    Unload();
    Load();
}

void GUI::CloseAll(){
    main_menu->Close();
    options_menu->Close();
    loading_menu->Close();
    ingame_menu->Close();
}

GUI* GUI::GetGUI(){return instance;}

Menu* GUI::GetMenu(int id){
    for(Menu* m: menus){if(id == m->id)return m;}
    return null;
}
    
void GUI::Paint(){
    for(Menu* m: menus){
        m->Paint();
    }
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
void GUI::OnSignal(EventSignal signal){
    for(Widget* w: debug_widgets){if(w->HandleSignal(signal))return;}
    for(Menu* m: menus){if(m->HandleSignal(signal))return;}
}
void GUI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.w=(float)screen_w;
    fullscreen_layout.h=(float)screen_h;
    fullscreen_layout.center.x=fullscreen_layout.w/2;
    fullscreen_layout.center.y=fullscreen_layout.h/2;
    for(Menu* m: menus){m->HandleResize();}
    for(Widget* w: debug_widgets){w->HandleResize();}
}