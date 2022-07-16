#include <game_project_1/client/client.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/log.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>
#include <game_project_1/client/client_net_handler.hpp>

#include <game_project_1/system/physics.hpp>
#include <game_project_1/system/movement.hpp>

#include <game_project_1/base/base_player.hpp>


Client* Client::instance = nullptr;
float Client::frame_interval=1;


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

    my_slot_id=-1;
    my_save_id=0;
    
    frame_interval = config::frame_interval;
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


Player* Client::Me(){
    if(my_slot_id<0)return nullptr;
    return players[my_slot_id];
}

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
            UpdateScene(frames_to_run,seconds);
            ClientNetHandler::SendPlayerDelta();
        }
        ui.Update(&scene,frames);
    }
    Input::Update();
    ClientNetwork::Update();
    ClientNetHandler::Update(frames);
    ClientSignalHandler::Update(frames);
}

void Client::UpdateScene(int frames,float delta){
    for(int i=0;i<frames;i++){
        for(ClientEntity* entity:scene.entities){
            Movement::Update(entity,frame_interval);
            Physics::ClientFrame(entity,&scene,frame_interval);
        }
    }

    scene.camera_manager.Update(delta);
    scene.global_timer+=frames;
}

void Client::OnSpawnPlayer(ClientEntity* player){
    logger::debug("Setting player control to entity %d\n",player->id);
    scene.SetPlayerControl(player->id);
    if(!ui.ingame_menu->active){ui.ingame_menu->Open();}
    if(ui.loading_menu->active){ui.loading_menu->Close();}
}

void Client::Quit(){
    Game::Exit();
}