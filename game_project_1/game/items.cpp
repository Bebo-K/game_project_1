#include <game_project_1/game/items.hpp>
#include <game_project_1/types/str.hpp>

Array<EquipSlot> EquipSlots;
Array<ItemType> ItemTypes;


EquipSlot::EquipSlot(){
    name=nullptr;
}
EquipSlot::~EquipSlot(){
    DEALLOCATE(name)
}
    
EquipSlot* EquipSlot::GetByName(char* name){
    for(EquipSlot* slot:EquipSlots){
        if(cstr::compare(slot->name,name)){
            return slot;
        }
    }
    return nullptr;
}

void EquipSlot::FromJson(JSONObject* json){
    name = cstr::new_copy(json->GetString("name")->string);
}

ItemInstance::ItemInstance(){type_id=0;stack_count=0;}
ItemInstance::~ItemInstance(){}
int ItemInstance::SerializedLength(){
    return sizeof(ItemInstance);
}
void ItemInstance::Read(Deserializer& dat){
    type_id = dat.GetInt();
    stack_count = dat.GetInt();
}
void ItemInstance::Write(Serializer& dat){
    dat.PutInt(type_id);
    dat.PutInt(stack_count);
}
void ItemInstance::Clear(){
    type_id=0;
    stack_count=0;
}


ItemType::ItemType(){name=nullptr;equip=nullptr;}
ItemType::~ItemType(){
    DEALLOCATE(name)
    if(equip != nullptr)delete equip;
    equip=nullptr;
}
void ItemType::FromJson(JSONObject* json){
    name = wstr::from_cstr(json->GetString("name")->string);
    max_stack = json->GetInt("max_stack");
    if(json->HasJObject("equip")){
        equip = new EquipInfo();
        equip->FromJson(json->GetJObject("equip"));
    }
    else{equip=nullptr;}
}


EquipInfo::EquipInfo(){stat_mod=nullptr;}
EquipInfo::~EquipInfo(){
    if(stat_mod != nullptr){delete stat_mod;}
}
void EquipInfo::FromJson(JSONObject* json){
    EquipSlot* slot = EquipSlot::GetByName(json->GetString("slot")->string);
    slot_id = (slot)? slot->id:-1;
    damage = json->GetInt("damage");
    armor = json->GetInt("armor");
    if(json->HasJObject("stat_mod")){
        stat_mod = new StatLayer();
        stat_mod->FromJson(json->GetJObject("stat_mod"));
    }
    else{stat_mod=nullptr;}
}