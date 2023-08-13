#include <game_project_1/component/component_loader.hpp>
#include <game_project_1/component/component.hpp>

#include <game_project_1/component/client/animation_state.hpp>
#include <game_project_1/component/client/model_set.hpp>
#include <game_project_1/component/client/sprite_set.hpp>

#include <game_project_1/component/server/persistence.hpp>
#include <game_project_1/component/server/npc_properties.hpp>
#include <game_project_1/component/server/npc_state.hpp>

#include <game_project_1/component/shared/action_state.hpp>
#include <game_project_1/component/shared/character.hpp>
#include <game_project_1/component/shared/collider_set.hpp>
#include <game_project_1/component/shared/equip.hpp>
#include <game_project_1/component/shared/identity.hpp>
#include <game_project_1/component/shared/inventory.hpp>
#include <game_project_1/component/shared/movement_properties.hpp>
#include <game_project_1/component/shared/movement_state.hpp>
#include <game_project_1/component/shared/physics_properties.hpp>
#include <game_project_1/component/shared/physics_state.hpp>
#include <game_project_1/component/shared/stat_block.hpp>

ComponentLoader* ComponentLoader::instance = null;

ComponentLoader* ComponentLoader::GetInstance(){
    if(ComponentLoader::instance == null){
        logger::fatal("No ComponentLoader instance was found");
    }
    return ComponentLoader::instance;
}

void ComponentLoader::SetInstance(ComponentLoader* new_instance){
    if(ComponentLoader::instance == null){
        instance = new_instance;
    }
    if(new_instance == null){
        instance = new_instance;
        return;
    }
    if( new_instance->SharedCount() > instance->SharedCount() ||
        new_instance->ClientCount() > instance->ClientCount() ||
        new_instance->ServerCount() > instance->ServerCount()
    ){
        logger::info("Replacing ComponentLoader instance");
        instance = new_instance;
    }
    else{
        logger::warn("Replacing ComponentLoader instance, but it contains less defined components!");
        instance = new_instance;
    }
}


BaseComponentLoader::BaseComponentLoader(){SetInstance(this);}

template<> int BaseComponentLoader::TypeID<Identity>(){return 0;}
template<> int BaseComponentLoader::TypeID<PhysicsProperties>(){return 1;}
template<> int BaseComponentLoader::TypeID<MovementProperties>(){return 2;}
//template<> int BaseComponentLoader::TypeID<ActionProperties>(){return 3;}
template<> int BaseComponentLoader::TypeID<PhysicsState>(){return 4;}
template<> int BaseComponentLoader::TypeID<MovementState>(){return 5;}
template<> int BaseComponentLoader::TypeID<ActionState>(){return 6;}
template<> int BaseComponentLoader::TypeID<ColliderSet>(){return 7;}
template<> int BaseComponentLoader::TypeID<StatBlock>(){return 8;}
template<> int BaseComponentLoader::TypeID<Equip>(){return 9;}
template<> int BaseComponentLoader::TypeID<Inventory>(){return 10;}
template<> int BaseComponentLoader::TypeID<Character>(){return 11;}

int BaseComponentLoader::SharedCount(){return 12;}
Component* BaseComponentLoader::NewSharedComponent(int ID){
    switch(ID){
        case 0: return new Identity();
        case 1: return new PhysicsProperties();
        case 2: return new MovementProperties();
        //case 3: return new ActionProperties();
        case 4: return new PhysicsState();
        case 5: return new MovementState();
        case 6: return new ActionState();
        case 7: return new ColliderSet();
        case 8: return new StatBlock();
        case 9: return new Equip();
        case 10: return new Inventory();
        case 11: return new Character();
        default: return null;
    }
}


int BaseComponentLoader::ClientCount(){return 3;}
Component* BaseComponentLoader::NewClientComponent(int ID){
    switch(ID){
        case 0: return new ModelSet();
        case 1: return new SpriteSet();
        case 2: return new AnimationState(-1);
        default: return null;
    }
}

int BaseComponentLoader::ServerCount(){return 3;}
Component* BaseComponentLoader::NewServerComponent(int ID){
    switch(ID){
        case 0: return new Persistence(-1);
        case 1: return new NPCProperties(-1);
        case 2: return new NPCState();
        default: return null;
    }
}




namespace SharedComponent{
    extern int Count;//How many component slots a Entity is created with

    template <typename T> const int TypeID = -1;

    template<> const int TypeID<Identity> =0;
    template<> const int TypeID<PhysicsProperties> =1;
    template<> const int TypeID<MovementProperties> =2;
    //template<> const int TypeID<ActionProperties>=3;
    template<> const int TypeID<PhysicsState> =4;
    template<> const int TypeID<MovementState> =5;
    template<> const int TypeID<ActionState> =6;
    template<> const int TypeID<ColliderSet> =7;
    template<> const int TypeID<StatBlock> =8;
    template<> const int TypeID<Equip> =9;
    template<> const int TypeID<Inventory> =10;
    template<> const int TypeID<Character> =11;

    template <int ID> Component* NewInstance(){return null;}
    Component* NewInstance(int ID);

    /*
    template<> Component* NewInstance<TypeID<Identity>>(){return new Identity();}
    template<> Component* NewInstance<TypeID<PhysicsProperties>>(){return new PhysicsProperties();}
    template<> Component* NewInstance<TypeID<MovementProperties>>(){return new MovementProperties();}
    //template<> Component* NewInstance<TypeID<ActionProperties>>(){return new ActionProperties();}
    template<> Component* NewInstance<TypeID<PhysicsState>>(){return new PhysicsState();}
    template<> Component* NewInstance<TypeID<MovementState>>(){return new MovementState();}
    template<> Component* NewInstance<TypeID<ActionState>>(){return new ActionState();}
    template<> Component* NewInstance<TypeID<ColliderSet>>(){return new ColliderSet();}
    template<> Component* NewInstance<TypeID<StatBlock>>(){return new StatBlock();}
    template<> Component* NewInstance<TypeID<Equip>>(){return new Equip();}
    template<> Component* NewInstance<TypeID<Inventory>>(){return new Inventory();}
    template<> Component* NewInstance<TypeID<Character>>(){return new Character();}
    */
};

namespace ClientComponent{
    extern int Count;//How many component slots a ClientEntity is created with

    template <typename T> const int TypeID = -1;

    template<> const int TypeID<ModelSet> =0;
    template<> const int TypeID<SpriteSet> =1;
    template<> const int TypeID<AnimationState> =2;


    template <int ID> Component* NewInstance(){return null;}

    template<> Component* NewInstance<TypeID<ModelSet>>(){return new ModelSet();}
    template<> Component* NewInstance<TypeID<SpriteSet>>(){return new SpriteSet();}
    template<> Component* NewInstance<TypeID<AnimationState>>(){return new AnimationState(-1);}
};

namespace ServerComponent{
    extern int Count;//How many component slots a ServerEntity is created with

    template <typename T> const int TypeID = -1;

    template<> const int TypeID<Persistence> =0;
    template<> const int TypeID<NPCProperties> =1;
    template<> const int TypeID<NPCState> =2;


    template <int ID> Component* NewInstance(){return null;}

    template<> Component* NewInstance<TypeID<Persistence>>(){return new Persistence(-1);}
    template<> Component* NewInstance<TypeID<NPCProperties>>(){return new NPCProperties(-1);}
    template<> Component* NewInstance<TypeID<NPCState>>(){return new NPCState();}
};

