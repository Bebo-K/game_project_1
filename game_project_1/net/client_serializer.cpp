#include <game_project_1/net/client_serializer.hpp>
#include <game_project_1/io/serializer.hpp>
#include <game_project_1/game/spawn.hpp>
#include <game_project_1/game/entity_serializer.hpp>

bitmask player_delta_mask = bitmask::of_bits(
    {Position::ComponentID, PhysicsState::ComponentID, MovementState::ComponentID, ActionState::ComponentID} ,4);

Client* client;
byte delta_buffer[Datagram::MAX_DATA_LENGTH];

void ClientSerializer::Init(Client* c){client=c;}
void ClientSerializer::Free(){client=nullptr;}

Payload ClientSerializer::ClientDelta(ClientEntity* entity){
    int entity_len = EntitySerializer::SerializedLength(entity,player_delta_mask);
    if(entity_len == 0){return Payload(PacketID::CDLT,0,nullptr);}
    int delta_length = sizeof(int) + entity_len;

    Payload ret(PacketID::CDLT,delta_length,delta_buffer);

    Serializer delta_data(ret.data,ret.length);
        delta_data.PutInt(entity->id);
        EntitySerializer::Serialize(entity,player_delta_mask,delta_data);

    return ret;
}

void ClientSerializer::DeserializeNewScene(ClientScene* scene, Payload payload){
    Deserializer scene_deserializer(payload.data);
    int new_area_id = scene_deserializer.GetInt();
    logger::debug("Recieved server scene transition to area %d\n",new_area_id);
    scene->Load(new_area_id);
    Player* me = client->Me();
    me->entity_id = scene_deserializer.GetInt();
    me->entity_scene = new_area_id;
    int entity_count = scene_deserializer.GetInt();
    for(int i=0;i<entity_count;i++){
        ClientEntity* new_entity = scene->AddEntity(scene_deserializer.GetInt());
        EntitySerializer::Deserialize(new_entity,scene_deserializer,payload.timestamp);
        scene->SpawnEntity(new_entity,SpawnType::APPEAR);
        if(new_entity->id == me->entity_id){client->OnSpawnPlayer(new_entity);}
    }
}

void ClientSerializer::DeserializeServerDelta(ClientScene* scene, Payload payload){
    Deserializer delta_deserializer(payload.data);
    int updated_entities = delta_deserializer.GetInt();
    for(int i=0;i<updated_entities;i++){
        int entity_id = delta_deserializer.GetInt();
        ClientEntity* e = scene->GetEntity(entity_id);
        if(e != null){
            if(e->id == client->Me()->entity_id){
                //TODO: reconcile server/client 
                EntitySerializer::Skip(delta_deserializer);//don't overwrite my player
            }
            else{
                EntitySerializer::Deserialize(e,delta_deserializer,payload.timestamp);
            }
        }
        else{
            ClientEntity* temp_entity = scene->AddEntity(entity_id);
            EntitySerializer::Deserialize(temp_entity,delta_deserializer,payload.timestamp);
        }
    }
}

void ClientSerializer::DeserializeSpawnEntities(ClientScene* scene, Payload payload){
    Deserializer spawn_deserializer(payload.data);
    int create_count = spawn_deserializer.GetInt();
    for(int i=0;i<create_count;i++){
        int entity_id = spawn_deserializer.GetInt();
        int spawn_type = spawn_deserializer.GetInt();
        ClientEntity* entity = scene->GetEntity(entity_id);
        if(entity == nullptr){
            entity = scene->AddEntity(entity_id);
            EntitySerializer::Deserialize(entity,spawn_deserializer,payload.timestamp);
            scene->SpawnEntity(entity,spawn_type);  
            if(entity->id == client->Me()->entity_id){client->OnSpawnPlayer(entity);}
        }
        else{
            logger::debug("Entity ID collision on spawn: %d\n",entity_id);
            EntitySerializer::Skip(spawn_deserializer);
        }
    }
}
void ClientSerializer::DeserializeDespawnEntities(ClientScene* scene, Payload payload){
    Deserializer despawn_deserializer(payload.data);
    int delete_count = despawn_deserializer.GetInt();
    for(int i=0;i<delete_count;i++){
        int entity_id = despawn_deserializer.GetInt();
        int despawn_type_id = despawn_deserializer.GetInt();
        //what to do if this is my entity? Unload scene?
        scene->DespawnEntity(entity_id,despawn_type_id);
    }
}