export module ClientScene;

import Map;
import Pool;

import Config;
import Logger;
import AssetManager;
import EntityTemplate;

import Renderer;

import ClientEntity;
import IdentityComponent;
import ModelSetComponent;
import SpriteSetComponent;

import ColliderSetComponent;
import HitBoxesComponent;


import ClientLevel;

import CameraManager;
import PlayerInput;
import AnimationController;


export class ClientScene{
public:
    int  area_id;
    long long global_timer;//Frame count since the scene has been loaded.

    Renderer renderer;
    CameraManager camera_manager;
    ClientLevel level;
    Pool<ClientEntity> entities;
    ClientEntity* my_player;

    ClientScene():renderer(),level(),entities(8){
        area_id=-1;
        global_timer=0;
        renderer.camera.ortho=false;
        my_player=nullptr;
    }
    ~ClientScene(){Unload();}
    void Load(int area){
        area_id=area;
        logger::info("loading client level for area id %d...\n",area);
        level.LoadArea(area);
        renderer.Add(&level);
        global_timer=1;
    }
    void Unload(){
        camera_manager.DetachCamera();
        area_id=-1;
        global_timer=0;
        my_player=nullptr;
        entities.Clear();
    }

    void Draw(){
        camera_manager.PreDraw();
        renderer.Draw();
    }

    ClientEntity* AddEntity(int eid){
        ClientEntity* entity = GetEntity(eid);
        if(entity==nullptr){
            entity = new (entities.Allocate()) ClientEntity(eid);
        }
        return entity;
    }
    ClientEntity* GetEntity(int eid){
        for(ClientEntity* e:entities){
            if(e->id == eid)return e;
        }
        return nullptr;
    }
    void DestroyEntity(int eid){
        for(int i=0;i<entities.Count();i++){
            if(entities[i]->id==eid){
                RemoveFromRender(entities[i]);
                entities.Delete(i);break;
            }
        }
    }

    void BuildEntity(ClientEntity* e){
        Identity* identity = e->Get<Identity>();
        if(identity != nullptr && identity->type != 0){
            EntityTemplate::Build(identity->type,e,this);
        }
    }
    void AddToRender(ClientEntity* e){
        if(e->Has<ModelSet>()){renderer.Add(e->Get<ModelSet>());}
        if(e->Has<SpriteSet>()){renderer.Add(e->Get<SpriteSet>());}
        if(config::debug_mode && e->Has<ColliderSet>()){renderer.Add(e->Get<ColliderSet>());}
        if(config::debug_mode && e->Has<HitBoxes>()){renderer.Add(e->Get<HitBoxes>());}
    }
    void RemoveFromRender(ClientEntity* e){
        if(e->Has<ModelSet>()){renderer.Remove(e->Get<ModelSet>());}
        if(e->Has<SpriteSet>()){renderer.Remove(e->Get<SpriteSet>());}
        if(e->Has<ColliderSet>()){renderer.Remove(e->Get<ColliderSet>());}
        if(e->Has<HitBoxes>()){renderer.Remove(e->Get<HitBoxes>());}
    }

    void SetPlayerControl(int entity_id){
        ClientEntity* player = GetEntity(entity_id);
        camera_manager.AttachCamera(&renderer.camera,player);
        my_player=player;
    }
    bool OnInput(Input::Event input){
        if(my_player != nullptr && PlayerInput::HandleInput(my_player,this,input)){return true;}
        else return camera_manager.HandleCameraInput(input);
    }

};