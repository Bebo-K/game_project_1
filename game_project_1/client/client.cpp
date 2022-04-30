#include <game_project_1/client/client.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/log.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>
#include <game_project_1/client/client_net_handler.hpp>

#include <game_project_1/base/base_player.hpp>


Client* Client::instance = nullptr;


Client::Client() : scene(), ui(),players(0){
    logger::info("Initializing client\n");
    Client::instance = this;
    ShaderManager::Init();
    TextureManager::Init();
    ModelManager::Init();
    AnimationManager::Init();  
    FontManager::Init();
    ModelManager::Init();
    ClientNetwork::Init();
    
    ClientNetHandler::Init(this);
    ClientSignalHandler::Init(this);

    my_entity_id=0;
    current_players=0;
}

Client::~Client(){
    for(int i=0;i<players.length;i++){players[i]->Clear();}
    ui.Unload();
    FontManager::Free();
    ModelManager::Free();
    ShaderManager::Free();
    TextureManager::Free();
    ModelManager::Free();
    AnimationManager::Free();
    ClientNetwork::Free();
    ClientNetHandler::Free();
    ClientSignalHandler::Free();
}

Client* Client::GetClient(){return Client::instance;}
void Client::Signal(EventSignal val){ClientSignalHandler::Signal(val);}

void Client::Start(){
    FontManager::LoadFontFace("Merriweather/Merriweather-Regular",8);
    FontID debug_font =FontManager::LoadFontFace("SourceSansPro-Regular",12);
    FontManager::SetActiveFont(debug_font);

    ClientInit_BasePak1();



    ui.Load();
    ui.main_menu->Open();
}

void Client::Paint(){
    scene.Draw();
    ui.Paint();
}

void Client::Resize(int screen_w,int screen_h){
    scene.renderer.camera.width=screen_w;
    scene.renderer.camera.height=screen_h;
    ui.OnResize(screen_w,screen_h);
}


void Client::Update(int frames){ 
    float seconds = frames*config::frame_interval;
    AnimationManager::Update(seconds);
    int frames_to_run = (frames < FRAMESKIP_MAX)?frames:FRAMESKIP_MAX;
    
    if(frames_to_run >0){
        for(Input::Event input = Input::NextEvent();input != Input::None;input = Input::NextEvent(input)){
            if(scene.OnInput(input)){Input::ClearEvent(input);continue;}
            if(ui.OnInput(input)){Input::ClearEvent(input);}
        }

        if(scene.global_timer > 0){
            scene.Update(frames_to_run,seconds);
            ClientNetHandler::SendPlayerDelta();
        }
        ui.Update(&scene,frames);
    }
    Input::Update();
    ClientNetwork::Update();
    ClientNetHandler::Update(frames);
    ClientSignalHandler::Update(frames);
}

void Client::Quit(){
    Game::Exit();
}