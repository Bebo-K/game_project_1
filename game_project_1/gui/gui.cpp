#include <game_project_1/gui/gui.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>


GUI* GUI::instance = nullptr;


using namespace UI;

GUI::GUI():menus(),developer_console(){
    GUI::instance=this;

    main_menu = new MainMenu();
    options_menu = new OptionsMenu();
    loading_menu = new LoadingMenu();
    error_menu = new ErrorMenu();
    ingame_menu = new IngameMenu();
    character_create_menu = new CharacterCreateMenu();

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

    developer_console.MoveTo(nullptr,Top,Left,{2,2});
    
    
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
void GUI::Update(ClientScene* scene,Timestep delta){
    for(Menu* m: menus){m->Update(delta);}
    developer_console.Update(delta);
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
    developer_console.HandleResize();
    for(Menu* m: menus){m->HandleResize();}
}

void GUI::DebugLog(wchar* str){
    DeveloperConsole::Write(str);
}