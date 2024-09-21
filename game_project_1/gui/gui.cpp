#include <game_project_1/gui/gui.hpp>
#include <game_project_1/os.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/io/log.hpp>
#include <math.h>


GUI* GUI::instance = nullptr;


using namespace UI;

GUI::GUI():menus(),developer_layer(){
    GUI::instance=this;

    developer_layer = new DeveloperLayer();
    main_menu = new MainMenu();
    options_menu = new OptionsMenu();
    loading_menu = new LoadingMenu();
    error_menu = new ErrorMenu();
    ingame_menu = new IngameMenu();
    character_create_menu = new CharacterCreateMenu();

    menus.Add(developer_layer);
    menus.Add(main_menu);
    menus.Add(options_menu);
    menus.Add(loading_menu);
    menus.Add(error_menu);
    menus.Add(ingame_menu);
    menus.Add(character_create_menu);
}

void GUI::Load(){
    logger::info("Loading GUI...");

    developer_layer->Load();
    main_menu->Load();
    options_menu->Load();
    loading_menu->Load();
    ingame_menu->Load();
    error_menu->Load();
    character_create_menu->Load();

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
    developer_layer->Close();
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
}
void GUI::Update(ClientScene* scene,Timestep delta){
    for(Menu* m: menus){m->Update(delta);}
}
bool GUI::OnInput(Input::Event event_type){
    Controller::Button toggle = Controller::GetToggleConsole();
    if(event_type == Input::Event::ToggleConsole && 
    toggle.IsJustPressed() &&
    !developer_layer->active &&
    config::enable_dev_console){
        developer_layer->active=true;
        developer_layer->visible=true;
        return true;
    }
    for(Menu* m: menus){if(m->HandleInput(event_type))return true;}
    return false;
}
void GUI::OnSignal(EventSignal signal){
    for(Menu* m: menus){if(m->HandleSignal(signal))return;}
}

void GUI::OnResize(int screen_w,int screen_h){
    for(Menu* m: menus){m->HandleResize();}
}