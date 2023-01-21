#include <game_project_1/component/logic_components.hpp>
#include <game_project_1/game/dice.hpp>
#include <stdlib.h>

StatBlock::StatBlock(){level=1;}
StatBlock::StatBlock(int start_level){level=start_level;}
StatBlock::StatBlock(StatBlock* s2){
    level=s2->level;
    base_stats.Copy(&s2->base_stats);
}
StatBlock::~StatBlock(){}
int StatBlock::SerializedLength(){return sizeof(int)+sizeof(BaseStats);}
void StatBlock::Read(Deserializer& dat){
    level=dat.GetInt();
    base_stats.max_hp=dat.GetInt();
    base_stats.max_hp=dat.GetInt();
    base_stats.strength=dat.GetInt();
    base_stats.intelligence=dat.GetInt();
    base_stats.agility=dat.GetInt();
}
void StatBlock::Write(Serializer& dat){
    dat.PutInt(level);
    dat.PutInt(base_stats.max_hp);
    dat.PutInt(base_stats.max_mp);
    dat.PutInt(base_stats.strength);
    dat.PutInt(base_stats.intelligence);
    dat.PutInt(base_stats.agility);
}



Equip::Equip(){}
Equip::Equip(Equip* e2){
    head = e2->head;
    body = e2->body;
    gloves = e2->gloves;
    boots = e2->boots;
    right_hand = e2->right_hand;
    left_hand = e2->left_hand;
}
Equip::~Equip(){}
int Equip::SerializedLength(){return sizeof(ItemInstance)*6;}
void Equip::Read(Deserializer& dat){
    head.Read(dat);
    body.Read(dat);
    gloves.Read(dat);
    boots.Read(dat);
    right_hand.Read(dat);
    left_hand.Read(dat);
}
void Equip::Write(Serializer& dat){
    head.Write(dat);
    body.Write(dat);
    gloves.Write(dat);
    boots.Write(dat);
    right_hand.Write(dat);
    left_hand.Write(dat);
}



Inventory::Inventory(){
    inventory_slots=0;
    items = null;
}
Inventory::Inventory(Inventory* i2){
    inventory_slots = i2->inventory_slots;
    if(inventory_slots != 0){
        items = (ItemInstance*)calloc(inventory_slots,sizeof(ItemInstance));
        for(int i=0;i<inventory_slots;i++){
            items[i] = i2->items[i];
        }
    }
}
Inventory::~Inventory(){
    if(items != null){free(items);items=null;inventory_slots=0;}
}
int Inventory::SerializedLength(){return sizeof(int)+sizeof(ItemInstance)*inventory_slots;}
void Inventory::Read(Deserializer& dat){
    if(items != null){free(items);items=null;inventory_slots=0;}
    inventory_slots = dat.GetInt();
    items = (ItemInstance*)calloc(inventory_slots,sizeof(ItemInstance));
    for(int i=0;i<inventory_slots;i++){items[i].Read(dat);}
}
void Inventory::Write(Serializer& dat){
    dat.PutInt(inventory_slots);
    for(int i=0;i<inventory_slots;i++){items[i].Write(dat);}
}



Character::Character(){race_id=0;class_id=0;}
Character::Character(Character* c2){
    race_id = c2->race_id;
    class_id = c2->class_id;
    appearance = c2->appearance;
}

int Character::SerializedLength(){return sizeof(int)*2 + sizeof(CharacterAppearance);}
void Character::Read(Deserializer& dat){
    race_id = dat.GetInt();
    class_id = dat.GetInt();
    appearance.style1 = dat.GetInt();
    appearance.style2 = dat.GetInt();
    appearance.style3 = dat.GetInt();
    appearance.color1.from_int(dat.GetInt());
}
void Character::Write(Serializer& dat){
    dat.PutInt(race_id);
    dat.PutInt(class_id);
    dat.PutInt(appearance.style1);
    dat.PutInt(appearance.style2);
    dat.PutInt(appearance.style3);
    dat.PutInt(appearance.color1.to_int());
}


NPCControllerState::NPCControllerState(NPCControllerType type){
    init=false;
    current_action=0;
    action_counter=0;
    controller_type = type;
}

Persistance::Persistance(){global_id =0;}