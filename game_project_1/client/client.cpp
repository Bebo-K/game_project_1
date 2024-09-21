#include <game_project_1/client/client.hpp>
#include <game_project_1/game.hpp>
#include <game_project_1/io/log.hpp>

#include <game_project_1/net/network.hpp>
#include <game_project_1/client/client_signal.hpp>
#include <game_project_1/client/client_net_handler.hpp>

#include <game_project_1/system/shared/physics.hpp>
#include <game_project_1/system/shared/movement.hpp>
#include <game_project_1/system/shared/combat.hpp>

#include <game_project_1/game/content.hpp>
#include <game_project_1/content/base_content.hpp>
#include <game_project_1/gfx/debugdraw.hpp>


Client* Client::instance = nullptr;
float Client::frame_interval=1;


Client::Client() : scene(), ui(),players(){
    logger::info("Initializing client\n");
    Client::instance = this;
    ShaderManager::Init();
    TextureManager::Init();
    ModelManager::Init();
    AnimationManager::Init();  
    FontManager::Init();
    ModelManager::Init();
    ClientNetwork::Init();
    DebugDraw::Init();
    
    ClientNetHandler::Init(this);
    ClientSignalHandler::Init(this);

    my_slot_id=-1;
    my_save_id=0;
    
    frame_interval = config::tick_interval;
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

    OnContentLoad();
    BaseContent::LoadClient();
    

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


void Client::Update(Timestep delta){ 
    AnimationManager::Update(delta);
    int frames_to_run = (delta.frames < FRAMESKIP_MAX)?delta.frames:FRAMESKIP_MAX;
    
    if(frames_to_run >0){
        for(Input::Event input = Input::NextEvent();input != Input::None;input = Input::NextEvent(input)){
            if(ui.OnInput(input)){Input::ClearEvent(input);continue;}
            if(scene.OnInput(input)){Input::ClearEvent(input);}
        }

        if(scene.global_timer > 0){
            UpdateScene({frames_to_run,frames_to_run*frame_interval});
            ClientNetHandler::SendPlayerDelta();
        }
        ui.Update(&scene,delta);
    }
    Input::Update();
    ClientNetwork::Update();
    ClientNetHandler::Update(delta);
    ClientSignalHandler::Update(delta);
}

void Client::UpdateScene(Timestep delta){
    for(int i=0;i<delta.frames;i++){
        for(ClientEntity* entity:scene.entities){
            Movement::Update(entity,{1,frame_interval});
            Physics::ClientUpdate(entity,&scene,{1,frame_interval});
            AnimationController::ClientUpdate(entity,{1,frame_interval});
            Combat::ClientUpdate(entity,{1,frame_interval});
        }
    }

    scene.camera_manager.Update(delta);
    scene.global_timer+=delta.frames;
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