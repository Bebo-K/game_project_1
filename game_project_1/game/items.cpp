#include <game_project_1/game/items.hpp>
#include <game_project_1/types/str.hpp>


Map<int,EquipSlot*> EquipSlots(4);
Map<int,ItemType*> ItemTypes(4);



EquipSlot::EquipSlot(){
    name=nullptr;
}
EquipSlot::~EquipSlot(){
    if(name != nullptr)free(name);
    name=nullptr;
}
    
int EquipSlot::GetIDByName(char* name){
    for(int i=0;i<EquipSlots.Count();i++){
        EquipSlot* slot = EquipSlots.ValueAtIndex(i);
        if(slot != nullptr && slot->name != nullptr 
        && cstr::compare(name,slot->name) ){
            return i;
        }
    }
    return -1;
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
    if(name != nullptr)free(name);
    name=nullptr;
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
    slot_id = EquipSlot::GetIDByName(json->GetString("slot")->string);
    damage = json->GetInt("damage");
    armor = json->GetInt("armor");
    if(json->HasJObject("stat_mod")){
        stat_mod = new StatLayer();
        stat_mod->FromJson(json->GetJObject("stat_mod"));
    }
    else{stat_mod=nullptr;}
}