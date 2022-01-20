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


GUI* GUI::instance = nullptr;

using namespace UI;

GUI::GUI(): fullscreen_layout(),menus(),developer_console(){
    GUI::instance=this;

    main_menu = new MainMenu(&fullscreen_layout);
    options_menu = new OptionsMenu(&fullscreen_layout);
    loading_menu = new LoadingMenu(&fullscreen_layout);
    error_menu = new ErrorMenu(&fullscreen_layout);
    ingame_menu = new IngameMenu(&fullscreen_layout);
    character_create_menu = new CharacterCreateMenu(&fullscreen_layout);

    menus.Add(main_menu);
    menus.Add(options_menu);
    menus.Add(loading_menu);
    menus.Add(error_menu);
    menus.Add(ingame_menu);
    menus.Add(character_create_menu);
}

void GUI::Load(){
    logger::info("Loading GUI...");

    developer_console.Load();
    main_menu->Load();
    options_menu->Load();
    loading_menu->Load();
    ingame_menu->Load();
    error_menu->Load();
    character_create_menu->Load();
    
    DeveloperConsole::Write(L"Hello World! GUI Initialized.");

    OnResize(Window::width,Window::height);
    logger::info("done\n");
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
    error_menu->Close();
    character_create_menu->Close();
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
    developer_console.Paint();
}
void GUI::Update(Scene* scene, int frames){
    for(Menu* m: menus){m->Update(frames);}
    developer_console.Update(frames);
}
bool GUI::OnInput(Input::Event event_type){
    if(developer_console.OnInput(event_type))return true;
    for(Menu* m: menus){if(m->HandleInput(event_type))return true;}
    return false;
}
void GUI::OnSignal(EventSignal signal){
    if(developer_console.HandleSignal(signal))return;
    for(Menu* m: menus){if(m->HandleSignal(signal))return;}
}
void GUI::OnResize(int screen_w,int screen_h){
    fullscreen_layout.w=(float)screen_w;
    fullscreen_layout.h=(float)screen_h;
    fullscreen_layout.center.x=fullscreen_layout.w/2;
    fullscreen_layout.center.y=fullscreen_layout.h/2;

    developer_console.layout.MoveTo(&fullscreen_layout,Top,Left,{2,2});
    developer_console.OnResize();
    for(Menu* m: menus){m->HandleResize();}
}

void GUI::DebugLog(wchar* str){
    DeveloperConsole::Write(str);
}