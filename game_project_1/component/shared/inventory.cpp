#include <game_project_1/component/shared/inventory.hpp>


Inventory::Inventory(){
    items=null;
    inventory_slots=0;
}

Inventory::~Inventory(){Clear();}

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

Component* Inventory::Clone(ComponentParentContext context){
    Inventory* copy = new Inventory();
    copy->inventory_slots = inventory_slots;
    if(inventory_slots != 0){
        copy->items = (ItemInstance*)calloc(inventory_slots,sizeof(ItemInstance));
        for(int i=0;i<inventory_slots;i++){
            copy->items[i] = items[i];
        }
    }
    return copy;
}

void Inventory::Clear(){
    DEALLOCATE(items)
    inventory_slots=0;
}