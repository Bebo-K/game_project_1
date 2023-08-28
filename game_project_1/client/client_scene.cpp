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
    player_input.Detach();
    area_id=-1;
    global_timer=0;
    entities.Clear();
}

void ClientScene::Draw(){
    camera_manager.PreDraw();
    for(ClientEntity* e:entities){        
        if(e->Has<ModelSet>()){e->Get<ModelSet>()->SetTransform(e->GetLocation(),e->scale);}
        if(e->Has<SpriteSet>()){e->Get<SpriteSet>()->SetTransform(e->GetLocation(),e->scale);}
    }
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
    for(ClientEntity* e:entities){if(e->id == eid)return e;}
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

void ClientScene::AddToRender(ClientEntity* e){
    if(e->Has<ModelSet>()){renderer.Add(e->Get<ModelSet>());}
    if(e->Has<SpriteSet>()){renderer.Add(e->Get<SpriteSet>());}
}

void ClientScene::RemoveFromRender(ClientEntity* e){
    if(e->Has<ModelSet>()){renderer.Remove(e->Get<ModelSet>());}
    if(e->Has<SpriteSet>()){renderer.Remove(e->Get<SpriteSet>());}
}

void ClientScene::SetPlayerControl(int entity_id){
    ClientEntity* player = GetEntity(entity_id);
    camera_manager.AttachCamera(&renderer.camera,player);
    player_input.Attach(player,&renderer.camera);
}
bool ClientScene::OnInput(Input::Event input){
    if(player_input.HandleInput(input)){return true;}
    else return camera_manager.HandleCameraInput(input);
}
