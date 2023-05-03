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
    template<> Component* NewInstance<TypeID<AnimationState>>(){return new AnimationState();}
};

namespace ServerComponent{
    extern int Count;//How many component slots a ServerEntity is created with


    template <typename T> const int TypeID = -1;

    template<> const int TypeID<Persistence> =0;
    template<> const int TypeID<NPCProperties> =1;
    template<> const int TypeID<NPCState> =2;


    template <int ID> Component* NewInstance(){return null;}

    template<> Component* NewInstance<TypeID<Persistence>>(){return new Persistence(null);}
    template<> Component* NewInstance<TypeID<NPCProperties>>(){return new NPCProperties();}
    template<> Component* NewInstance<TypeID<NPCState>>(){return new NPCState();}
};

