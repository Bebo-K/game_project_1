#ifndef ITEM_H
#define ITEM_H

#include <game_project_1/io/serializer.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/game/stats.hpp>

class ItemInstance{
    public:
    int type_id;
    int stack_count;
    //int hone_level,hone_durability;

    ItemInstance();
    ~ItemInstance();
    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Clear();
};

struct EquipSlot{
    int id;
    char* name;
    EquipSlot();
    ~EquipSlot();
    static EquipSlot* GetByName(char* name);
    void FromJson(JSONObject* json);
};

struct EquipInfo{
    int slot_id;
    int damage;
    int armor;
    StatLayer* stat_mod;
    //on_equip
    //on_unequip
    //on_attack
    //on_take_damage

    EquipInfo();
    ~EquipInfo();
    void FromJson(JSONObject* json);
};

struct ItemType{
    wchar* name;
    //wchar* plural_name
    int max_stack;
    EquipInfo* equip;
    ItemType();
    ~ItemType();
    void FromJson(JSONObject* json);

    //on_use
    //on_pickup
    //on_remove
};

extern Array<EquipSlot> EquipSlots;
extern Array<ItemType> ItemTypes;

#endif