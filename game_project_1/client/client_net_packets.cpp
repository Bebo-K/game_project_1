#include <game_project_1/client/client_net_handler.hpp>
#include <game_project_1/io/serializer.hpp>

using namespace ClientNetHandler;

void ClientNetHandler::ParseNewScenePacket(ClientScene* scene, Payload payload){
    Deserializer scene_in(payload.data);
    int new_area_id = scene_in.GetInt();
    logger::debug("Recieved server scene transition to area %d\n",new_area_id);
    scene->Load(new_area_id);
    Player* me = client->Me();
    me->entity_id = scene_in.GetInt();
    me->entity_scene = new_area_id;
    int entity_count = scene_in.GetInt();
    for(int i=0;i<entity_count;i++){
        ClientEntity* new_entity = scene->AddEntity(scene_in.GetInt());
        new_entity->Read(scene_in,payload.timestamp);
        scene->AddToRender(new_entity);
        if(new_entity->id == me->entity_id){client->OnSpawnPlayer(new_entity);}
    }
}

void ClientNetHandler::ParseServerDeltaPacket(ClientScene* scene, Payload payload){
    Deserializer delta_in(payload.data);
    int updated_entities = delta_in.GetInt();
    for(int i=0;i<updated_entities;i++){
        int entity_id = delta_in.GetInt();
        ClientEntity* e = scene->GetEntity(entity_id);
        if(e != null){
            if(e->id == client->Me()->entity_id){
                //TODO: reconcile server/client 
                Entity::SkipRead(delta_in);//don't overwrite my player
            }
            else{
                e->Read(delta_in,payload.timestamp);
            }
        }
        else{
            ClientEntity* temp_entity = scene->AddEntity(entity_id);
            temp_entity->Read(delta_in,payload.timestamp);
        }
    }
}

void ClientNetHandler::ParseSpawnEntitiesPacket(ClientScene* scene, Payload payload){
    Deserializer spawn_in(payload.data);
    int create_count = spawn_in.GetInt();
    for(int i=0;i<create_count;i++){
        int entity_id = spawn_in.GetInt();
        int spawn_type = spawn_in.GetInt();
        ClientEntity* entity = scene->GetEntity(entity_id);
        if(entity == nullptr){
            entity = scene->AddEntity(entity_id);
            entity->Read(spawn_in,payload.timestamp);
            scene->AddToRender(entity);  
            if(entity->id == client->Me()->entity_id){client->OnSpawnPlayer(entity);}
        }
        else{
            logger::debug("Entity ID collision on spawn: %d\n",entity_id);
            Entity::SkipRead(spawn_in);
        }
    }
}

void ClientNetHandler::ParseDespawnEntitiesPacket(ClientScene* scene, Payload payload){
    Deserializer despawn_in(payload.data);
    int delete_count = despawn_in.GetInt();
    for(int i=0;i<delete_count;i++){
        int entity_id = despawn_in.GetInt();
        int despawn_type_id = despawn_in.GetInt();
        //what to do if this is my entity? Unload scene?
        scene->DestroyEntity(entity_id);
    }
}