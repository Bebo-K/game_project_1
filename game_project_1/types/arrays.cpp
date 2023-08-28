#include <game_project_1/types/arrays.hpp>
#include <game_project_1/io/log.hpp>
#include <string.h>

//////////////////////////
//////***BitArray***/////
////////////////////////
BitArray::BitArray(){
    bits = 0;
    data = null;
}
BitArray::BitArray(int bit_count){
    Initialize(bit_count);
}
BitArray::~BitArray(){
    free(data);
}
void BitArray::Resize(int new_bit_count){
    if(new_bit_count ==0){
        free(data);data=null;
        bits=0;
        return;
    }
    int old_char_count = bits/8 + (bits % 8 > 0);
    int new_char_count = new_bit_count/8 + (new_bit_count % 8 > 0);
    int new_int_count = new_char_count/4 + ((new_char_count % 4) > 0);
    if(allocated < (int)(new_int_count*sizeof(int))){
        allocated = (int)(new_int_count*sizeof(int));
        unsigned char* new_data = (unsigned char*)calloc(new_char_count,1);
        memcpy(new_data,data,old_char_count);
        
        free(data);
        data = new_data;
    }
    bits = new_bit_count;
}
void BitArray::Initialize(int bit_count){
    bits = bit_count;
    int char_count = bit_count/8 + ((bit_count % 8) > 0);
    int int_count = char_count/4 + ((char_count % 4) > 0);
    allocated = sizeof(int)*int_count;//try to fit on int boundary
    data = (unsigned char*)calloc(allocated,1);
}
int BitArray::CountBitsSet(){
    int count = 0;
    for(int bit=0;bit<bits;bit++){
        int byte = bit/8;
        int bit_indx = bit%8;
        if((data[byte] & (1 << bit_indx)) != 0)count++;
    }
    return count;
}
int BitArray::CountBitsUnset(){
    int count = 0;
    for(int bit=0;bit<bits;bit++){
        int byte = bit/8;
        int bit_indx = bit%8;
        if((data[byte] & (1 << bit_indx)) == 0)count++;
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
    byte dat = data[index/8];
    byte mask = ~(1 << (index%8));
    dat = dat & mask;
    data[index/8] = dat;
}
bool BitArray::Toggle(int index){
    if(index < 0 || index >= bits){logger::exception("BitArray::toggle -> Index is out of range: %d",index);}

    data[index/8] = (data[index/8] ^ (1 << (index%8)));
    return ((data[index/8] >> (index%8)) & 1) > 0;
}
void BitArray::Clear(){
    int bytes = bits/8 + ((bits % 8) > 0);
    memset(data,0,bytes);
}
void BitArray::Print(){
    for(int bit=0;bit<bits;bit++){
        int byte = bit/8;
        int bit_indx = bit%8;
        logger::info("%s:%c ",bit, ((data[byte] & (1 << bit_indx)) != 0)? 'T':'F');
    }
}

int BitArray::GetSizeInBytes(){
    int bitflag_size_bytes = (bits/8);
    bitflag_size_bytes += (bitflag_size_bytes%8 > 0)?1:0;
    return bitflag_size_bytes;
}
int BitArray::GetSizeInInts(){
    int bitflag_size_ints = (bits/32);
    bitflag_size_ints += (bitflag_size_ints%32 > 0)?1:0;
    return bitflag_size_ints;
}

/////////////////////////////
//////***IntegerSet***//////
///////////////////////////
IntegerSet::IntegerSet(){
    allocated=2;
    data=(int*)malloc(sizeof(int)*allocated);
    length=0;
}
IntegerSet::IntegerSet(int size){
    allocated=size;
    length=size;
    data= (size==0)? null:(int*)malloc(sizeof(int)*allocated);
}
IntegerSet::~IntegerSet(){Clear();}

void IntegerSet::Add(int entry){
    if(Has(entry)){return;}
    if(length+1 > allocated){
        allocated= (allocated==0)? 2:allocated*2;
        int* new_data = (int*)malloc(sizeof(int)*allocated);
        for(int i=0;i<length;i++){new_data[i] = data[i];}
        free(data);
        data=new_data;
    }
    data[length]=entry;
    length++;
}
bool IntegerSet::Has(int entry){
    for(int i=0;i<length;i++){if(data[i]==entry){return true;}}
    return false;
}
void IntegerSet::Remove(int entry){
    if(!Has(entry)){return;}
    int index =0;
    for(int i=0;i<length;i++){if(data[i]==entry){index=i;}}
    if(index != length-1){data[index]=data[length-1];}
    length--;
}
void IntegerSet::Clear(){
    if(data != null){free(data);data=null;}
    length=0;
    allocated=0;
}

int IntegerSet::operator[](int index){
    return data[index];
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
void* DynamicArray::Get(int index){
    if(occupancy.Get(index)){return &data[index*slot_size];}else return null;
}
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

