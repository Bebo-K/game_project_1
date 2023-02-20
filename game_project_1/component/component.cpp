#include <game_project_1/component/component.hpp>

#include <game_project_1/component/anim_components.hpp>
#include <game_project_1/component/gfx_components.hpp>
#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/component/state_components.hpp>

#include <game_project_1/log.hpp>


SharedComponent* NewInstance(int component_id){
    switch (component_id)
    {
    case 0: return new Identity(0,null);
    case 1: return new Position({0,0,0},{0,0,0},{0,0,0});
    case 2: return new PhysicsProperties();
    case 3: return new MoveProperties();
    //case 4: return new ActionProperties();
    case 5: return new PhysicsState();
    case 6: return new MovementState();
    case 7: return new ActionState();
    case 8: return new ColliderSet();
    case 9: return new StatBlock();
    case 10:return new Equip();
    case 11:return new Inventory();
    case 12:return new Character();
    default: break;
    }
    //TODO: extensible Client/Server/SharedComponents
    logger::exception("Could not instantiate shared component ID %d",component_id);
    return null;
}

ClientComponent* NewInstance(int component_id){
    switch (component_id)
    {
    case 0: return new ModelSet();
    case 1: return new SpriteSet();
    case 2: return new AnimationState();
    default: break;
    }
    logger::exception("Could not instantiate client component ID %d",component_id);
    return null;
}

ServerComponent* NewInstance(int component_id){
    switch (component_id)
    {
    case 0: return new ServerDelta();
    case 1: return new Persistance();
    case 2: return new NPCControllerState();
    default: break;
    }
    logger::exception("Could not instantiate server component ID %d",component_id);
    return null;
}



Identity::Identity(EntityClass Type, wchar* Name){
    type=Type;
    name=wstr::new_copy(Name);
}
Identity::~Identity(){Clear();}
void Identity::Clear(){
    type=0;
    if(name != null){free(name);name=null;}
}
int Identity::SerializedLength(){
    return sizeof(int)+sizeof(wchar)*(wstr::len()+1);
}
void Identity::Read(Deserializer& dat){
    type = dat.GetInt();
    name = dat.GetWString();
}
void Identity::Write(Serializer& dat){
    dat.PutInt(type);
    dat.PutWString(name);
}


Position::Position(vec3 pos,vec3 rot,vec3 vel){
    x=pos.x;
    y=pos.y;
    z=pos.z;
    rotation=rot;
    velocity=vel;
    scale = {1.0f,1.0f,1.0f};
}
Position::~Position(){}
void Position::Reset(){
    x=y=z=0.0f;
    rotation = {0.0f,0.0f,0.0f};
    velocity = {0.0f,0.0f,0.0f};
    scale = {1.0f,1.0f,1.0f};
}
int Position::SerializedLength(){return sizeof(float)*12;}
void Position::Read(Deserializer& dat){
    x=dat.GetFloat(); y=dat.GetFloat(); z=dat.GetFloat();
    rotation = {dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
    velocity = {dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
    scale = {dat.GetFloat(),dat.GetFloat(),dat.GetFloat()};
}
void Position::Write(Serializer& dat){
    dat.PutFloat(x); dat.PutFloat(y); dat.PutFloat(z);
    dat.PutFloat(rotation.x); dat.PutFloat(rotation.y); dat.PutFloat(rotation.z);
    dat.PutFloat(velocity.x); dat.PutFloat(velocity.y); dat.PutFloat(velocity.z);
    dat.PutFloat(scale.x); dat.PutFloat(scale.y); dat.PutFloat(scale.z);
}



ServerDelta::ServerDelta(){Clear();}
ServerDelta::~ServerDelta(){Clear();}
void ServerDelta::Clear(){
    changed_components =bitmask(0);
    last_update_position = {0.0f,0.0f,0.0f};
    last_update_velocity = {0.0f,0.0f,0.0f};
}