#ifndef LOGIC_COMPONENTS_H
#define LOGIC_COMPONENTS_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/game/item.hpp>
#include <game_project_1/game/stats.hpp>
#include <game_project_1/game/races_and_classes.hpp>
#include <game_project_1/types/arrays.hpp>


//RPG stat block
class StatBlock: public SharedComponent{
	public:
    static int ComponentID = 9;

    int level;
    BaseStats base_stats;

    StatBlock();
    StatBlock(int start_level);
    StatBlock(StatBlock* s2);
    ~StatBlock();

    inline int ID(){return StatBlock::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

 //Only equipment is sent thru client delta
class Equip: public SharedComponent{
	public:
    static int ComponentID = 10;

    ItemInstance head;
    ItemInstance body;
    ItemInstance gloves;
    ItemInstance boots;
    ItemInstance right_hand;
    ItemInstance left_hand;

    Equip();
    Equip(Equip* s2);
    ~Equip();

    inline int ID(){return Equip::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

//Inventory is private unless requested
class Inventory: public SharedComponent{
	public:
    static int ComponentID = 11;

    int inventory_slots;
    ItemInstance* items;

    Inventory();
    Inventory(Inventory* i2);
    ~Inventory();

    inline int ID(){return Inventory::ComponentID;}
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};



//Public character info: race, character class, appearance
class Character: public SharedComponent{
    public:
    RaceID race_id;
    ClassID class_id;
    CharacterAppearance appearance;

    Character();
    Character(Character* c2);
    ~Character();
    
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


//Assigns a globally unique ID 
class Persistance: public ServerComponent{
    public: 
    const static int ServerComponentID = 1;

    int global_id;//unique across all scenes+entities, corresponds to a SaveEntity

    Persistance();
    ~Persistance();
    inline int ID(){return Persistance::ServerComponentID;}
};

//NPC "brains"
typedef int NPCControllerType;
class NPCControllerState: public ServerComponent{
    public: 
    const static int ServerComponentID = 2;

    NPCControllerType controller_type;
    bool init;//1-time-init control path flag that the controller callback can optionally use
    int current_action;
    int action_counter;
    
    Array<value> constants;
    Array<value> parameters;

    NPCControllerState(NPCControllerType type);
    ~NPCControllerState();
    inline int ID(){return Persistance::ServerComponentID;}
};


#endif