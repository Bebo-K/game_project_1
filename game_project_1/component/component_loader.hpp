#include <game_project_1/component/component.hpp>

class ComponentLoader{
    private:
    static ComponentLoader* instance;
    protected:
    void SetInstance(ComponentLoader* loader);
    virtual int ClientCount() = 0;
    virtual int ServerCount() = 0;
    virtual int SharedCount() = 0;

    virtual Component* InitClientComponent(int ID) = 0;
    virtual Component* InitServerComponent(int ID) = 0;
    virtual Component* InitSharedComponent(int ID) = 0;
    
    public:
    static ComponentLoader* GetInstance();

    static int ClientCount();
    static int ServerCount();
    static int SharedCount();

    static Component* NewClientComponent(int ID);
    static Component* NewServerComponent(int ID);
    static Component* NewSharedComponent(int ID);
};

class BaseComponentLoader: public ComponentLoader{
    public:
    BaseComponentLoader();

    //template <typename T> int TypeID = -1;
    
    template <typename T> int TypeID();
    virtual int ClientCount();
    virtual int ServerCount();
    virtual int SharedCount();
    
    virtual Component* NewClientComponent(int ID);
    virtual Component* NewServerComponent(int ID);
    virtual Component* NewSharedComponent(int ID);
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

    //template <int ID> Component* NewInstance(){return null;}
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

