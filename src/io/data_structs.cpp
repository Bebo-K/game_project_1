#include "log.h"
#include "data_structs.h"
#include <stdlib.h>
#include <string.h>


template <typename T>
Array<T>::Array(){}

template <typename T>
int Array<T>::count(){return running_count;}

template <typename T>
int Array<T>::add(T* object){return 0;}

template <typename T>
T* Array<T>::get(int index){return null;}

template <typename T>
T* Array<T>::remove(int index){return null;}

DataBlock::DataBlock(int slot_count,int slot_size){
    slot_is_filled=BitArray(slots);
    data = calloc(slots,slot_size);
}

int DataBlock::count(){
    return slot_is_filled.set_count();
}
int DataBlock::add(void* object){
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!slot_is_filled.get(i)){
            added_slot =i;
            memcpy((char*)data+(added_slot*slot_size),object,slot_size);
            slot_is_filled.set(added_slot);
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        resize(slots*2);
        memcpy((char*)data+(added_slot*slot_size),object,slot_size);
        slot_is_filled.set(added_slot);
    }
    return added_slot;
}
void* DataBlock::get(int index){
    return (char*)data+(index*slot_size);
}
void DataBlock::remove(int index){
    slot_is_filled.unset(index);
    memset ((char*)data+(index*slot_size),0,slot_size);
}
void DataBlock::resize(int new_count){
    //TODO: implement
    return;
}

BitArray::BitArray(){
    bits = 0;
    data = null;
}

BitArray::BitArray(int bit_count){
    bits = bit_count;
    int char_count = (bit_count/8) + (((bit_count % 8) > 0)?1:0);

    data = (unsigned char*)calloc(char_count,1);
}

BitArray::~BitArray(){
    free(data);
}

void BitArray::resize(int new_bit_count){
    int old_char_count = bits/8 + (bits % 8 > 0)?1:0;
    int new_char_count = new_bit_count/8 + (new_bit_count % 8 > 0)?1:0;
    unsigned char* new_data = (unsigned char*)calloc(new_char_count,1);
    
    memcpy(new_data,data,old_char_count);
    bits = new_bit_count;
    free(data);
    data = new_data;
}

int BitArray::set_count(){
    int count = 0;
    for(int i=0; i< bits/8;i++){
        for(int j=0;j<8;j++){
            count += ((data[i] >> j) & 1);
        }
    }
    return count;
}
int BitArray::unset_count(){
    int count = 0;
    for(int i=0; i< bits/8;i++){
        for(int j=0;j<8;j++){
            count += ((data[i] >> j) ^ 1);
        }
    }  
    return count;
}
bool BitArray::get(int index){
    if(index < 0 || index > bits){log("BitArray::is_set -> Index is out of range: %d",index);}
    return ((data[index/8] >> (index%8)) & 1) > 0;
}
void BitArray::set(int index){
    if(index < 0 || index > bits){log("BitArray::set -> Index is out of range: %d",index);}
    data[index/8] = (data[index/8] | (1 << (index%8)));
}
void BitArray::unset(int index){
    if(index < 0 || index > bits){log("BitArray::unset -> Index is out of range: %d",index);}
    data[index/8] = (data[index/8] &  ~(1 << (index%8)));
}

bool BitArray::toggle(int index){
    if(index < 0 || index > bits){log("BitArray::toggle -> Index is out of range: %d",index);}

    data[index/8] = (data[index/8] ^ (1 << (index%8)));
    return ((data[index/8] >> (index%8)) & 1) > 0;
}

