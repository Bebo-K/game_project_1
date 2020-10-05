#include "client.h"
#include "../game.h"
#include "../log.h"
#include <stdio.h>
#include "../game/systems/camera_manager.h"
#include "../game/systems/entity_collision.h"
#include "../game/systems/movement.h"
#include "../game/systems/npc_controller.h"
#include "../game/systems/physics.h"
#include "../game/systems/player_input.h"
#include "../game/systems/transition_manager.h"
//#include "../game/systems/animation_controller.h"
#include "../game/systems/state_manager.h"
#include "../gfx/text.h"


Client::Client() : scene(), scene_renderer(), ui(){}




void Client::Start(){
    logger::info("Initializing client...\n");
    ShaderManager::AddShader("basic_lighting","dat/gfx/basic_lighting.vrt","dat/gfx/basic_lighting.frg");
    ShaderManager::AddShader("scene_debug","dat/gfx/scene_debug.vrt","dat/gfx/scene_debug.frg");
    ShaderManager::AddShader("shadeless","dat/gfx/shadeless.vrt","dat/gfx/shadeless.frg");
    ShaderManager::AddShader("skybox","dat/gfx/skybox.vrt","dat/gfx/skybox.frg");
    ShaderManager::AddShader("flat_skybox","dat/gfx/flat_skybox.vrt","dat/gfx/flat_skybox.frg");
    FontManager::Init();
        //FontID DebugFont = FontManager::LoadFontFace("dat/ui/fonts/Merriweather/Merriweather-Regular.ttf",8);
        FontID DebugFont = FontManager::LoadFontFace("dat/ui/fonts/SourceSansPro-Regular.ttf",12);
    ModelManager::Init();
        ModelManager::Register(PLAYER,"dat/models/gargoyle.glb");

    ui.Load();
    scene_renderer.Load();
    scene_renderer.camera.SetShader("basic_lighting");
    
    scene_renderer.camera.ortho=false;
    scene_renderer.camera.z = 10.0f;
    scene_renderer.camera.y = 2.0f;

    SimpleText* textbox = new SimpleText(TextString::from_cstr("The quick brown fox!"),DebugFont);
    textbox->x = 100;
    textbox->y = 100;

    ui.renderer.Add(textbox);
    
    Texture tex;
    tex.atlas_id = FontManager::GetActiveFont()->atlas_gl_id;
    tex.tex_coords = {0,0,1,1};
    tex.width_px=1024;
    tex.height_px=1024;

    ShapePrimitive* box = new ShapePrimitive(CUBE,tex,5,5,5);
    scene_renderer.Add(box);
    
    LoadScene(0);
    if(scene.level.entrance_count > 0){
        SpawnPlayer(scene.level.entrances[0]);
    }
    else{
        Entrance default_spawn;
            default_spawn.name="default";
            default_spawn.position={0,0,0};
            default_spawn.style=0;
            default_spawn.target_pos={0,0,0};
        SpawnPlayer(default_spawn);
    }
}


void Client::LoadScene(int scene_id){
    scene.Load();
    scene_renderer.Add(&scene.level);
}

void Client::AddEntity(int eid){
    Entity *e = scene.AddEntity(eid);
    if(e->models != null){scene_renderer.Add(e->models);}    
}

void Client::RemoveEntity(int eid){
    Entity *e = scene.GetEntity(eid);
    if(e->models != null){scene_renderer.Remove(e->models);}
    scene.RemoveEntity(eid);
}

void Client::SpawnPlayer(Entrance eid){
    my_player = scene.AddEntity(0);
    my_player->type = 1;
    my_player->name= cstr::new_copy("Chowzang");
    my_player->state = new State();
        my_player->state->Set(IDLE);
    my_player->anim_state = new AnimationState(GROUND_UNIT);
    my_player->models = new ModelSet();
        my_player->models->Add(PLAYER);
    my_player->phys_data = new PhysicsData();
        my_player->phys_data->world_hitsphere.height=3;
        my_player->phys_data->world_hitsphere.radius=0.75f;
    my_player->colliders = new ColliderSet();
        //my_player->colliders->Add();
    my_player->movement = new MovementData();
        my_player->movement->base_speed=10;
        my_player->movement->jump_enabled=true;
    my_player->player_data = new PlayerData();
    my_player->camera_target = new CameraTarget(&scene_renderer.camera,{0,4,12},{0,1},{20,40});
    my_player->unit_data = new UnitData();
    PlayerInput::Track(my_player);

    if(my_player->models != null){scene_renderer.Add(my_player->models);}  


}


void Client::Paint(){
    CameraManager::Update(&scene,0);
    scene_renderer.Draw();
    ui.Paint();
}

void Client::UpdatePositions(){
    for(Entity* e:scene.entities){        
        if(e->models != null){
            e->models->x=e->x;
            e->models->y=e->y;
            e->models->z=e->z;
            e->models->rotation=e->rotation;
        }
    }
}

void Client::Update(int frames){
    float frame_interval = Game::FrameInterval();
    float seconds = frames*frame_interval;
        AnimationManager::Update(seconds);

    int frames_to_run = (frames < FRAMESKIP_MAX)?frames:FRAMESKIP_MAX;
    scene.global_timer+= frames_to_run;

    for(int frame=0;frame<frames_to_run;frame++){
        PlayerInput::Update(&scene,frame_interval);
        //NPCController::Update(&scene);
        //EntityCollision::Update(&scene);
        Movement::Update(&scene,frame_interval);
        Physics::FrameUpdate(&scene,frame_interval); 
        //TransitionManager::Update(&scene,ms);
        StateManager::Update(&scene,frame_interval);
        UpdatePositions();
        ui.Update(&scene,frames);
    }
    //Network::RunSync(scene);
}