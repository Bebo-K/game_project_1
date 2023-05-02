#include <game_project_1/server/server_net_handler.hpp>

#include <game_project_1/component/shared/inventory.hpp>

//Components that should be sent to clients by default when loading into a scene or spawning new entities
bitmask server_initial_sync_components = bitmask::invert(bitmask::of(SharedComponent::TypeID<Inventory>()));
//Components that should be seen by clients when updated server-side
bitmask server_delta_components = bitmask::invert(bitmask::of(SharedComponent::TypeID<Inventory>()+1));

Payload ServerNetHandler::WriteFullScene(Player* player_for,ServerScene* scene){
    int scene_size = sizeof(int)*3;//area_id, player_entity, num_entities
    int current_entities=0;

    for(ServerEntity* e:scene->entities){//First loop through to count entities and size
        scene_size += sizeof(int);//id
        scene_size += e->SerializedLength(server_initial_sync_components);
        current_entities++;
    }

    Payload scene_payload(PacketID::SCNE,scene_size,(byte*)calloc(scene_size,1));
    Serializer scene_out(scene_payload.data,scene_size);
        scene_out.PutInt(scene->area_id);
        scene_out.PutInt(player_for->entity_id);
        scene_out.PutInt(current_entities);

    for(ServerEntity* e:scene->entities){//Second loop to populate with spawn info
        scene_out.PutInt(e->id);
        e->Write(scene_out,server_initial_sync_components);
    }
    return scene_payload;
}

Payload ServerNetHandler::WriteSceneNewEntities(ServerScene* scene){
    int spawned_entities=scene->just_spawned.Count();
    if(spawned_entities==0){return Payload(PacketID::SPWN,0,0);}

    int spawn_size = sizeof(int);//num_entities
    for(ServerEntity* e:scene->just_spawned){//First loop through to count spawned entities and size
        spawn_size += sizeof(int);//id
        spawn_size += e->SerializedLength(server_initial_sync_components);
    }

    Payload spawn_payload(PacketID::SPWN,spawn_size,(byte*)calloc(spawn_size,1));
    Serializer spawn_out(spawn_payload.data,spawn_size);
        spawn_out.PutInt(spawned_entities);
    for(ServerEntity* e:scene->just_spawned){//Second loop to populate with spawn info
        spawn_out.PutInt(e->id);
        e->Write(spawn_out,server_initial_sync_components);
    }
    scene->just_spawned.Clear();
    return spawn_payload;
}

Payload ServerNetHandler::WriteSceneDeletedEntities(ServerScene* scene){
    int deleted_entities=scene->just_deleted.Count();
    if(deleted_entities==0){return Payload(PacketID::DSPN,0,0);}

    int delete_size = sizeof(int) + sizeof(int)*deleted_entities; //num_entities + (deleted)*{id}
    Payload delete_payload(PacketID::DSPN,delete_size,(byte*)calloc(delete_size,1));

    Serializer delete_out(delete_payload.data,delete_size);
        delete_out.PutInt(deleted_entities);

    for(ServerEntity* e:scene->just_deleted){
        delete_out.PutInt(e->id);
        scene->ClearEntity(e);
    }
    scene->just_deleted.Clear();
    return delete_payload;
}

Payload ServerNetHandler::WriteSceneDelta(ServerScene* scene){
    int delta_size = sizeof(int);//num_entities
    int updated_entities=0;

    for(ServerEntity* e:scene->entities){//First loop through to count changed entities and size
        bitmask client_visible_delta_mask = server_delta_components.and_with(e->changed_components);
        if(client_visible_delta_mask.val){
            delta_size += sizeof(int);//id
            delta_size += e->SerializedLength(client_visible_delta_mask);
            updated_entities++;
        }
    }
    if(updated_entities==0){return Payload(PacketID::DLTA,0,0);}

    Payload delta_payload(PacketID::DLTA,delta_size,(byte*)calloc(delta_size,1));
    Serializer delta_out(delta_payload.data,delta_size);
        delta_out.PutInt(updated_entities);

    for(ServerEntity* e:scene->entities){//Second loop to populate with delta info
        bitmask client_visible_delta_mask = server_delta_components.and_with(e->changed_components);
        if(client_visible_delta_mask.val){
            delta_out.PutInt(e->id);
            e->Write(delta_out,client_visible_delta_mask);
            e->ClearDelta();
        }
    }
    return delta_payload;
}

void ServerNetHandler::ParseClientDelta(Player* player_from,ServerScene* scene,Payload payload){
    Deserializer player_delta(payload.data);
    int entity_id = player_delta.GetInt();
    if(entity_id != player_from->entity_id){
        logger::warn("Client is sending a delta for an entity they're not. Ours:%d Theirs:%d\n",player_from->entity_id,entity_id);
    }
    ServerEntity* player_entity = scene->GetEntity(entity_id);
    player_entity->Read(player_delta,payload.timestamp);
}

