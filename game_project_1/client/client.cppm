export module Client;

import Logger;
import Timestep;
import Input;
import Array;
import Renderer;
import DebugDraw;
import GUI;
import ClientScene;
import Physics;
import Movement;
import Combat;
import Player;

import Network;
import Signal;
import ClientSignal;
import ClientNetwork;
//?
import Content;
import BaseContent;
//

export class Client{
private:
    static class Client* instance = nullptr;
    float frame_interval = 1.0f;
    int max_frames_per_update = 5;
public:

    ClientScene     scene;
    GUI             ui;


    Client() : scene(), ui(){
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
        //max_frames_per_update = config::max_client_frames_per_update;
    }

    ~Client(){
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

    static Client* GetClient(){return Client::instance;}

    void Start(){
        FontManager::LoadFontFace("Merriweather/Merriweather-Regular",8);
        FontID debug_font =FontManager::LoadFontFace("SourceSansPro-Regular",12);
        FontManager::SetActiveFont(debug_font);

        OnContentLoad();
        BaseContent::LoadClient();

        ui.Load();
        ui.main_menu->Open();
    }

    void Paint(){
        scene.Draw();
        ui.Paint();
    }
    void Resize(int screen_w,int screen_h){
        scene.renderer.camera.width=screen_w;
        scene.renderer.camera.height=screen_h;
        ui.OnResize(screen_w,screen_h);
    }
    void Update(Timestep delta)(Timestep delta){
        AnimationManager::Update(delta);
        int frames_to_run = (delta.frames > max_frames_per_update)? max_frames_per_update : delta.frames;

        if(frames_to_run >0){
            for(Input::Event input = Input::NextEvent();input != Input::Event::None;input = Input::NextEvent(input)){
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
    }

    void UpdateScene(Timestep delta){
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

    //TODO: this feels out of place
    void OnSpawnPlayer(ClientEntity* player){
    }
};

