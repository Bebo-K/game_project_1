#include <game_project_1/client/client_scene.hpp>
#include <game_project_1/io/asset_manager.hpp>

#include <game_project_1/core/entity_template.hpp>

#include <game_project_1/component/shared/identity.hpp>
#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/sprite_set.hpp>


ClientScene::ClientScene():renderer(),level(),entities(8){
    area_id=-1;
    global_timer=0;
    renderer.camera.ortho=false;
    my_player=nullptr;
}
ClientScene::~ClientScene(){Unload();}

void ClientScene::Load(int area){
    area_id=area;
    logger::info("loading client level for area id %d...\n",area);
    level.LoadArea(area);
    renderer.Add(&level);
    global_timer=1;
}

void ClientScene::Unload(){
    camera_manager.DetachCamera();
    area_id=-1;
    global_timer=0;
    my_player=nullptr;
    entities.Clear();
}

void ClientScene::Draw(){
    camera_manager.PreDraw();
    renderer.Draw();
}

ClientEntity* ClientScene::AddEntity(int eid){
    ClientEntity* entity = GetEntity(eid);
    if(entity==nullptr){
        entity = new (entities.Allocate()) ClientEntity(eid);
    }
    return entity;
}
ClientEntity* ClientScene::GetEntity(int eid){
    for(ClientEntity* e:entities){
        if(e->id == eid)return e;
         }
    return null;
}
void ClientScene::DestroyEntity(int eid){
    for(int i=0;i<entities.Count();i++){
        if(entities[i]->id==eid){
            RemoveFromRender(entities[i]);
            entities.Delete(i);break;
        }
    }
}

void ClientScene::BuildEntity(ClientEntity* e){
    Identity* identity = e->Get<Identity>();
    if(identity != null && identity->type != 0){
        EntityTemplate::Build(identity->type,e,this);
    }
}

void ClientScene::AddToRender(ClientEntity* e){
    if(e->Has<ModelSet>()){renderer.Add(e->Get<ModelSet>());}
    if(e->Has<SpriteSet>()){renderer.Add(e->Get<SpriteSet>());}
    if(config::debug_mode && e->Has<ColliderSet>()){renderer.Add(e->Get<ColliderSet>());}
    if(config::debug_mode && e->Has<HitBoxes>()){renderer.Add(e->Get<HitBoxes>());}
}

void ClientScene::RemoveFromRender(ClientEntity* e){
    if(e->Has<ModelSet>()){renderer.Remove(e->Get<ModelSet>());}
    if(e->Has<SpriteSet>()){renderer.Remove(e->Get<SpriteSet>());}
    if(e->Has<ColliderSet>()){renderer.Remove(e->Get<ColliderSet>());}
    if(e->Has<HitBoxes>()){renderer.Remove(e->Get<HitBoxes>());}
}

void ClientScene::SetPlayerControl(int entity_id){
    ClientEntity* player = GetEntity(entity_id);
    camera_manager.AttachCamera(&renderer.camera,player);
    my_player=player;
}
bool ClientScene::OnInput(Input::Event input){
    if(my_player != null && PlayerInput::HandleInput(my_player,this,input)){return true;}
    else return camera_manager.HandleCameraInput(input);
}
