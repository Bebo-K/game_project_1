#include <game_project_1/game/item.hpp>


ItemInstance::ItemInstance(){type=0;stack_count=0;}
ItemInstance::~ItemInstance(){}
int ItemInstance::SerializedLength(){
    return sizeof(ItemInstance);
}
void ItemInstance::Read(Deserializer& dat){
    type = dat.GetInt();
    stack_count = dat.GetInt();
}
void ItemInstance::Write(Serializer& dat){
    dat.PutInt(type);
    dat.PutInt(stack_count);
}