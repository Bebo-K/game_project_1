#include <game_project_1/net/server_serializer.hpp>
#include <game_project_1/game/entity_serializer.hpp>

ComponentMask server_delta_client_mask = ~(1 << 11/*Inventory*/);

Payload ServerSerializer::FullScene(Player* player_for,ServerScene* scene){
    int scene_size = sizeof(int)*3;//area_id, player_entity, num_entities
    int current_entities=0;
    int client_visible_entity_mask;

    for(ServerEntity* e:scene->entities){//First loop through to count entities and size
        client_visible_entity_mask = EntitySerializer::AllExistingComponents(e) & server_delta_client_mask;
        if(client_visible_entity_mask){
            scene_size += sizeof(int);//id
            scene_size += EntitySerializer::SerializedLength(e,client_visible_entity_mask);
            current_entities++;
        }
    }
    Payload scene_payload(PacketID::SCNE,scene_size,(byte*)calloc(scene_size,1));

    Serializer scene_serializer(scene_payload.data,scene_size);
    scene_serializer.PutInt(scene->area_id);
    scene_serializer.PutInt(player_for->entity_id);
    scene_serializer.PutInt(current_entities);

    for(ServerEntity* e:scene->entities){//Second loop to populate with spawn info
        client_visible_entity_mask = EntitySerializer::AllExistingComponents(e) & server_delta_client_mask;
        if(client_visible_entity_mask){//Update
            scene_serializer.PutInt(e->id);
            EntitySerializer::Serialize(e,client_visible_entity_mask,scene_serializer);
        }
    }
    return scene_payload;
}

Payload ServerSerializer::SceneNewEntities(ServerScene* scene){
    int spawn_size = sizeof(int);//num_entities
    int spawned_entities=0;
    int client_visible_spawn_mask;

    for(ServerEntity* e:scene->just_spawned){//First loop through to count spawned entities and size
        spawn_size += sizeof(int)*2;//id, spawn_type
        client_visible_spawn_mask = EntitySerializer::AllExistingComponents(e) & server_delta_client_mask;
        spawn_size += EntitySerializer::SerializedLength(e,client_visible_spawn_mask);
        spawned_entities++;
    }
    if(spawned_entities==0){return Payload(PacketID::SPWN,0,0);}

    Payload spawn_payload(PacketID::SPWN,spawn_size,(byte*)calloc(spawn_size,1));

    Serializer spawn_serializer(spawn_payload.data,spawn_size);
    spawn_serializer.PutInt(spawned_entities);

    for(ServerEntity* e:scene->just_spawned){//Second loop to populate with spawn info
        client_visible_spawn_mask = EntitySerializer::AllExistingComponents(e) & server_delta_client_mask;
        spawn_serializer.PutInt(e->id);
        spawn_serializer.PutInt(e->spawn_mode);
        EntitySerializer::Serialize(e,client_visible_spawn_mask,spawn_serializer);
        e->spawn_mode=0;
    }
    scene->just_spawned.Clear();
    return spawn_payload;
}

Payload ServerSerializer::SceneDeletedEntities(ServerScene* scene){
    int deleted_entities=scene->just_deleted.Count();
    if(deleted_entities==0){return Payload(PacketID::DSPN,0,0);}

    int delete_size = sizeof(int) + sizeof(int)*2*deleted_entities; //num_entities + (deleted)*{id, delete_style}
    Payload delete_payload(PacketID::DSPN,delete_size,(byte*)calloc(delete_size,1));

    Serializer delete_serializer(delete_payload.data,delete_size);
    delete_serializer.PutInt(deleted_entities);

    for(ServerEntity* e:scene->just_deleted){//Second loop to populate with deleted ids
        delete_serializer.PutInt(e->id);
        delete_serializer.PutInt(-e->spawn_mode);
        scene->ClearEntity(e);
    }
    scene->just_deleted.Clear();
    return delete_payload;
}

Payload ServerSerializer::SceneDelta(ServerScene* scene){
    int delta_size = sizeof(int);//num_entities
    int updated_entities=0;
    int client_visible_delta_mask;

    for(ServerEntity* e:scene->entities){//First loop through to count changed entities and size
        client_visible_delta_mask = e->delta_mask & server_delta_client_mask;
        if(client_visible_delta_mask){
            delta_size += sizeof(int);//id
            delta_size += EntitySerializer::SerializedLength(e,client_visible_delta_mask);
            updated_entities++;
        }
    }
    if(updated_entities==0){return Payload(PacketID::DLTA,0,0);}

    Payload delta_payload(PacketID::DLTA,delta_size,(byte*)calloc(delta_size,1));

    Serializer delta_serializer(delta_payload.data,delta_size);
    delta_serializer.PutInt(updated_entities);

    for(ServerEntity* e:scene->entities){//Second loop to populate with delta info
        client_visible_delta_mask = e->delta_mask & server_delta_client_mask;
        if(client_visible_delta_mask){//Update
            delta_serializer.PutInt(e->id);
            EntitySerializer::Serialize(e,client_visible_delta_mask,delta_serializer);
            e->delta_mask=0;
        }
    }
    return delta_payload;
}


void ServerSerializer::DeserializeClientDelta(Player* player_from,ServerScene* scene,Payload payload){
    Deserializer player_delta(payload.data);
    int entity_id = player_delta.GetInt();
    if(entity_id != player_from->entity_id){
        logger::warn("Client is sending a delta for an entity they're not. Ours:%d Theirs:%d\n",player_from->entity_id,entity_id);
    }
    ServerEntity* player_entity = scene->GetEntity(entity_id);
    EntitySerializer::Deserialize(player_entity,player_delta,payload.timestamp);
}

