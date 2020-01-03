#include "log.h"
#include "data_structs.h"
#include <stdlib.h>
#include <string.h>

const int DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE = 4;

BitArray::BitArray(){
    bits = 0;
    data = null;
}
BitArray::BitArray(int bit_count){
    bits = bit_count;
    int char_count = bit_count/8 + ((bit_count % 8) > 0);

    data = (unsigned char*)calloc(char_count,1);
}
BitArray::~BitArray(){
    free(data);
}
void BitArray::Resize(int new_bit_count){
    int old_char_count = bits/8 + (bits % 8 > 0);
    int new_char_count = new_bit_count/8 + (new_bit_count % 8 > 0);
    unsigned char* new_data = (unsigned char*)calloc(new_char_count,1);
    
    memcpy(new_data,data,old_char_count);
    bits = new_bit_count;
    free(data);
    data = new_data;
}
int BitArray::CountBitsSet(){
    int count = 0;
    for(int i=0; i< bits/8;i++){
        for(int j=0;j<8;j++){
            count += ((data[i] >> j) & 1);
        }
    }
    return count;
}
int BitArray::CountBitsUnset(){
    int count = 0;
    for(int i=0; i< bits/8;i++){
        for(int j=0;j<8;j++){
            count += ((data[i] >> j) ^ 1);
        }
    }  
    return count;
}
bool BitArray::Get(int index){
    if(index < 0 || index >= bits){logger::exception("BitArray::is_set -> Index is out of range: %d",index);}
    return ((data[index/8] >> (index%8)) & 1) > 0;
}
void BitArray::Set(int index){
    if(index < 0 || index >= bits){logger::exception("BitArray::set -> Index is out of range: %d",index);}
    data[index/8] = (data[index/8] | (1 << (index%8)));
}
void BitArray::Unset(int index){
    if(index < 0 || index >= bits){logger::exception("BitArray::unset -> Index is out of range: %d",index);}
    data[index/8] = (data[index/8] &  ~(1 << (index%8)));
}
bool BitArray::Toggle(int index){
    if(index < 0 || index >= bits){logger::exception("BitArray::toggle -> Index is out of range: %d",index);}

    data[index/8] = (data[index/8] ^ (1 << (index%8)));
    return ((data[index/8] >> (index%8)) & 1) > 0;
}

DataArray::DataArray(int object_size){
    slots=DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE;
    slot_size=object_size;
    slot_is_filled=BitArray(slots);
    data = (byte*)calloc(slots,slot_size);
}

DataArray::DataArray(int count,int object_size){
    slots=count;
    slot_size=object_size;
    slot_is_filled=BitArray(slots);
    data = (byte*)calloc(slots,slot_size);
}

int DataArray::Add(){
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!slot_is_filled.Get(i)){
            added_slot =i;
            break;
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        slot_is_filled.Resize(slots*2);
        Resize(slots*2);
    }
    slot_is_filled.Set(added_slot);
    return added_slot;
}

int DataArray::Add(void* object){
    int added_slot = Add();
    memcpy((char*)data+(added_slot*slot_size),object,slot_size);
    return added_slot;
}
void DataArray::Remove(int index){
    if(index < 0 || index >= slots){logger::exception("DataArray::Remove -> Index %d is out of range.",index);}
    slot_is_filled.Unset(index);
    memset ((char*)data+(index*slot_size),0,slot_size);
}
void* DataArray::Get(int index){
    if(index < 0 || index >= slots){logger::exception("DataArray::Get -> Index %d is out of range.",index);}
    if(!slot_is_filled.Get(index)){return null;}
    return (char*)data+(index*slot_size);
}
byte* DataArray::GetArray(){
    return data;
}
int DataArray::Count(){
    return slot_is_filled.CountBitsSet();
}
void DataArray::Resize(int new_count){
    byte* new_data = (byte*)calloc(new_count,slot_size);
    memcpy(new_data,data,slots*slot_size);
    slots = new_count;
    free(data);
    data = new_data;
    return;
}
PointerArray::PointerArray(){
    slots=DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE;
    data= (byte**)calloc(slots,sizeof(byte*));
}
PointerArray::PointerArray(int size){
    slots=size;
    data= (byte**)calloc(slots,sizeof(byte*));
}
int PointerArray::Add(void* object){
    int slot_to_add=-1;
    for(int i=0;i<slots;i++){
        if(data[i] == null){slot_to_add = i;break;}
    }
    if(slot_to_add == -1){
        slot_to_add = slots;
        Resize(slots*2);
    }
    data[slot_to_add] = (byte*)object;
    return slot_to_add;
}
void* PointerArray::Remove(int index){
    if(index < 0 || index >= slots){logger::exception("PointerArray::Remove -> Index %d is out of range.",index);}
    byte* ret = data[index];
    data[index] = null;
    return ret;
}
int PointerArray::Remove(void* object){
    for(int i=0;i<slots;i++){
        if(data[i] == object){
            data[i] = null;
            return i;
        }
    }
    logger::warn("PointerArray::Remove -> Object not found in array.");
    return -1;
}
void* PointerArray::Get(int index){
    if(index < 0 || index >= slots){logger::exception("PointerArray::Get -> Index %d is out of range.",index);}
    return data[index];
}
int PointerArray::Count(){
    int count =0;
    for(int i=0;i<slots;i++){
        if(data[i]!= null){count++;}
        }
    return count;
}
void PointerArray::Resize(int newsize){
    byte** newdata = (byte**)calloc(newsize,sizeof(byte*));
    for(int i=0;i<slots;i++){newdata[i] = data[i];}
    free(data);
    slots=newsize;
    data=newdata;
}