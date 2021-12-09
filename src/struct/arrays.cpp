#include "arrays.h"

#include "../log.h"
#include <string.h>

bool GET_BIT(int x,int bit){return ((x & (1 << bit)) != 0);}



//////////////////////////
//////***BitArray***/////
////////////////////////
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
    
    if(old_char_count <= new_char_count){
        memcpy(new_data,data,old_char_count);
    }
    bits = new_bit_count;
    free(data);
    data = new_data;
}
void BitArray::Initialize(int bit_count){
    bits = bit_count;
    int char_count = bit_count/8 + ((bit_count % 8) > 0);
    data = (unsigned char*)calloc(char_count,1);
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
void BitArray::Clear(){
    int char_count = bits/8 + ((bits % 8) > 0);
    memset(data,0,char_count);
}


/////////////////////////////
/////***DynamicArray***/////
///////////////////////////
DynamicArray::DynamicArray(int block_size):occupancy(1){
    slots=1;
    slot_size=block_size;
    data = (byte*)calloc(slots,slot_size);
}
DynamicArray::DynamicArray(int count,int block_size):occupancy(count){
    slots=count;
    slot_size=block_size;
    data = (byte*)calloc(slots,slot_size);
}
DynamicArray::~DynamicArray(){//occupancy array is auto-deallocated 
    free(data);
}
void* DynamicArray::Allocate(){
    int slot_to_add=0;
    while(slot_to_add < slots && occupancy.Get(slot_to_add)){slot_to_add++;}
    if(slot_to_add == slots){Resize(slots*2);}
    occupancy.Set(slot_to_add);
    return data + (slot_to_add*slot_size);
}
int DynamicArray::Add(void* object){
    int slot_to_add=0;
    while(slot_to_add < slots && occupancy.Get(slot_to_add)){slot_to_add++;}
    if(slot_to_add == slots){Resize(slots*2);}
    occupancy.Set(slot_to_add);
    memcpy(data + (slot_to_add*slot_size),object,slot_size);
    return slot_to_add;
}
void DynamicArray::Remove(int index){
    if(index < 0 || index >= slots){logger::exception("DynamicArray::Remove -> Index %d is out of range.",index);}
    memset(&data[index*slot_size],0,slot_size);
    occupancy.Unset(index);
}

int  DynamicArray::Index(byte* obj){
    if(obj < data || obj >= data+(slot_size*slots)){logger::exception("DynamicArray::Index -> Memory address 0x%08x is out of range.",obj);}
    return (obj-data)/slot_size;
}
void* DynamicArray::Get(int index){return &data[index*slot_size];}
int   DynamicArray::NextNonEmpty(int start_index){
    int next=start_index;
    while( ++next < slots && !occupancy.Get(next));
    return next;
}
int DynamicArray::Count(){return occupancy.CountBitsSet();}
void DynamicArray::Resize(int new_count){
    occupancy.Resize(new_count);
    byte* new_data = (byte*)calloc(new_count,slot_size);
    int slots_to_copy = (slots < new_count)?slots:new_count;
    memcpy(new_data,data,slots_to_copy*slot_size);
    slots = new_count;
    free(data);
    data = new_data;
}
void DynamicArray::Clear(){
    memset(data,0,slot_size*slots);
    occupancy.Clear();
}
DynamicArrayIterator DynamicArray::begin(){return {this,NextNonEmpty(-1)};}
DynamicArrayIterator DynamicArray::end(){return {this,slots};}


byte* DynamicArrayIterator::operator*(){
    return &parent->data[index*parent->slot_size];
}
DynamicArrayIterator DynamicArrayIterator::operator++(){
    index = parent->NextNonEmpty(index);
    return (*this);
}
bool DynamicArrayIterator::operator==(DynamicArrayIterator& l2){return index ==l2.index;}
bool DynamicArrayIterator::operator!=(DynamicArrayIterator& l2){return index !=l2.index;}
