#include <game_project_1/server/server_entity.hpp>


ServerEntity::ServerEntity(){
    id =0;
    entity_class_id =0;
    name=nullptr;
    x=y=z=0;
    velocity={0,0,0};
    scale ={1,1,1};
    rotation = {0,0,0};

    phys_data=null;
    colliders=null;
    movement=null;
    player_data=null;
    state=null;
    anim_state=null;
    unit_data=null;
    npc_data=null;
}
ServerEntity::~ServerEntity(){
    if(name != nullptr){free(name);name=nullptr;}
    if(phys_data != nullptr){delete phys_data;phys_data=nullptr;}
    if(colliders != nullptr){delete colliders;colliders=nullptr;}
    if(movement != nullptr){delete movement;movement=nullptr;}
    if(state != nullptr){delete state;state=nullptr;}
    if(player_data != nullptr){delete player_data;player_data=nullptr;}
    if(unit_data != nullptr){delete unit_data;unit_data=nullptr;}
    if(npc_data != nullptr){delete  npc_data;npc_data=nullptr;}
}
vec3 ServerEntity::GetPos(){return {x,y,z};}

int ServerEntity::DeltaLength(){return 0;}
void ServerEntity::SerializeDelta(byte* data){}
int ServerEntity::SerializedLength(){return 0;}
void ServerEntity::Serialize(byte* data){}
