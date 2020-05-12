#include "../log.h"
#include "data_Types.h"
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

DataArray::DataArray():occupancy(){
    slots=0;
    slot_size=0;
    data=null;
}
DataArray::DataArray(int object_size):occupancy(DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE){
    slots=DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
}

DataArray::DataArray(int count,int object_size):occupancy(count){
    slots=count;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
}
DataArray::~DataArray(){
    free(data);
}

void* DataArray::Add(){
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!occupancy.Get(i)){
            added_slot =i;
            break;
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        Resize(slots*2);
    }
    occupancy.Set(added_slot);
    return (char*)data+(added_slot*slot_size);
}
int DataArray::Add(void* object){
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!occupancy.Get(i)){
            added_slot =i;
            break;
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        Resize(slots*2);
    }
    occupancy.Set(added_slot);
    memcpy((char*)data+(added_slot*slot_size),object,slot_size);
    return added_slot;
}
void DataArray::Remove(int index){
    if(index < 0 || index >= slots){logger::exception("DataArray::Remove -> Index %d is out of range.",index);}
    occupancy.Unset(index);
    memset ((char*)data+(index*slot_size),0,slot_size);
}
void* DataArray::Get(int index){
    if(index < 0 || index >= slots){logger::exception("DataArray::Get -> Index %d is out of range.",index);}
    if(!occupancy.Get(index)){return null;}
    return (char*)data+(index*slot_size);
}
byte* DataArray::GetArray(){
    return data;
}
int DataArray::Count(){
    return occupancy.CountBitsSet();
}
void DataArray::Resize(int new_count){
    occupancy.Resize(new_count);
    byte* new_data = (byte*)calloc(new_count,slot_size);
    if(slots <= new_count){
        memcpy(new_data,data,slots*slot_size);
    }
    slots = new_count;
    free(data);
    data = new_data;
}
void DataArray::Clear(){
    memset(data,0,slot_size*slots);
    occupancy.Clear();
}
void DataArray::Initialize(int count,int object_size){
    slots=count;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
    occupancy.Initialize(count);
}


PointerArray::PointerArray(){
    slots=0;
    data=null;
}
PointerArray::PointerArray(int size){
    slots=size;data=null;
    if(size > 0){
        data= (byte**)calloc(slots,sizeof(byte*));
    }
}
PointerArray::~PointerArray(){
    if(data != nullptr){
        free(data); 
        data=nullptr;
    }
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
    if(newsize == 0){
        Clear();
        return;
    }
    byte** newdata = (byte**)calloc(newsize,sizeof(byte*));
    if(slots <= newsize){
        for(int i=0;i<slots;i++){newdata[i] = data[i];}
    }
    free(data);
    slots=newsize;
    data=newdata;
}
void PointerArray::Clear(){
    if(data != nullptr){
        free(data); 
        data=nullptr;
    }
    slots=0;
}

AssociativeArray::AssociativeArray(int initial_size):slot_is_filled(initial_size){
    slots=initial_size;
    key_data= (u_associative_array_key*)calloc(slots,sizeof(u_associative_array_key));
    value_data= (byte**)calloc(slots,sizeof(byte*));
}
AssociativeArray::~AssociativeArray(){
    free(key_data);
    free(value_data);
}

bool AssociativeArray::Add(int key,byte* value){
    u_associative_array_key u_key; u_key.intvalue=key;
    if(IndexOf(u_key) >= 0)return false;//already in map.
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!slot_is_filled.Get(i)){
            added_slot =i;
            break;
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        Resize(slots*2);
    }
    slot_is_filled.Set(added_slot);
    key_data[added_slot] = u_key;
    value_data[added_slot] = value;
    return added_slot;
}
bool AssociativeArray::Add(byte* key,byte* value){
    u_associative_array_key u_key; u_key.ptrvalue=key;
    if(IndexOf(u_key) >= 0)return false;//already in map.
    int added_slot = -1;
    for(int i=0;i < slots; i++){
        if(!slot_is_filled.Get(i)){
            added_slot =i;
            break;
        }
    }
    if(added_slot == -1){
        added_slot = slots;
        Resize(slots*2);
    }
    slot_is_filled.Set(added_slot);
    key_data[added_slot] = u_key;
    value_data[added_slot] = value;
    return added_slot;
}
byte* AssociativeArray::Remove(int key){
    u_associative_array_key u_key; u_key.intvalue=key;
    int value_index = IndexOf(u_key);
    if(value_index >= 0){
        slot_is_filled.Unset(value_index);
        return value_data[value_index];
    }
    return null;
}
byte* AssociativeArray::Remove(byte* key){
    u_associative_array_key u_key; u_key.ptrvalue=key;
    int value_index = IndexOf(u_key);
    if(value_index >= 0){
        slot_is_filled.Unset(value_index);
        return value_data[value_index];
    }
    return null;
}
byte* AssociativeArray::Get(int index){
    u_associative_array_key key; key.intvalue =index;
    int value_index = IndexOf(key);
    return (value_index >= 0)?value_data[value_index]:null;
}
byte* AssociativeArray::Get(byte* index){
    u_associative_array_key key; key.ptrvalue=index;
    int value_index = IndexOf(key);
    return (value_index >= 0)?value_data[value_index]:null;
}
int   AssociativeArray::IndexOf(u_associative_array_key key){
    for(int i=0; i<slots;i++){
        if(slot_is_filled.Get(i)==true){
            if(key_data[i].ptrvalue == key.ptrvalue ||
                key_data[i].intvalue == key.intvalue){
                    return i;
                }
        }
    }
    return -1;
}
byte* AssociativeArray::StrGet(const char* index){
    for(int i=0; i<slots;i++){
        if(slot_is_filled.Get(i)==true){
            if((char*)key_data[i].ptrvalue == index||
                strcmp((char*)key_data[i].ptrvalue,index)==0){
                    return value_data[i];
                }
        }
    }
    return null;
}
byte* AssociativeArray::StrRemove(const char* index){
    for(int i=0; i<slots;i++){
        if(slot_is_filled.Get(i)==true){
            if((char*)key_data[i].ptrvalue == index||
                strcmp((char*)key_data[i].ptrvalue,index)==0){
                    slot_is_filled.Unset(i);
                    return value_data[i];
                }
        }
    }
    return null;
}
int AssociativeArray::Count(){
    return slot_is_filled.CountBitsSet();
}

void AssociativeArray::Resize(int new_count){
    slot_is_filled.Resize(new_count);
    byte** new_value_data = (byte**)calloc(new_count,sizeof(byte*));
    byte** new_key_data = (byte**)calloc(new_count,sizeof(u_associative_array_key));
    if(slots <= new_count){
        memcpy(new_value_data,value_data,sizeof(byte*)*slots);
        memcpy(new_key_data,key_data,sizeof(u_associative_array_key)*slots);   
    }
    free(value_data);
    free(key_data);
    value_data=new_value_data;
    key_data=(u_associative_array_key*)new_key_data;
    slots=new_count;
}


char* cstr::new_copy(const char* old_string){
    if(old_string==nullptr)return nullptr;
    int str_len = strlen(old_string);
    char* str = (char*)malloc(str_len+1);
    strcpy(str,old_string);
    str[str_len]=0;
    return str;
}

char* cstr::lowercase_copy(const char* old_string){
    int str_len = strlen(old_string);
    char* str = (char*)malloc(str_len+1);
    strcpy(str,old_string);
    str[str_len]=0;
    
    for(int i=0;str[i]!=0;i++){
        if(str[i] >= 'A' && str[i] <= 'Z')str[i] += ('A'-'a');
    }
    return str;
}


bool cstr::compare(const char* str1,const char* str2){
    if(str1 == str2)return true;//pointer comparison shortcut
    return strcmp(str1,str2)==0;
}

bool cstr::starts_with(const char* str, const char* start){
    int i=0;
    for(i=0;str[i] != 0 && start[i]!= 0;i++){
        if(str[i] != start[i])return false;
    }
    if(str[i]==0 && start[i] != 0)return false;
    return true;
}

char* cstr::append(const char* str1,const char* str2){
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    char* str = (char*)malloc(str1_len+str2_len+1);
    strcpy(str,str1);
    strcpy(&str[str1_len],str1);
    str[str1_len+str2_len]=0;
    return str;
}

char* cstr::append(const char* str1, char seperator, const char* str2){
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    char* str = (char*)malloc(str1_len+str2_len+2);
    strcpy(str,str1);
    str[str1_len]=seperator;
    strcpy(&str[str1_len+1],str1);
    str[str1_len+str2_len+1]=0;
    return str;
}

char* cstr::utf16_to_utf8(const wchar_t* longstring){
    if(longstring==nullptr){return nullptr;}
    int utf_8_len=1;
    //loop through twice, first to get length of code points
    for(int i=0;longstring[i] != 0;i++){
        int code = longstring[i];
        if(code > 0xD7FF && (code > 0xFFFF || code < 0xE000)){//If 0xD800-0xDF00, it's 2 wchars wide
            code = 0x10000 + ((0xD800-longstring[i]) << 10) + (0XDC00 -longstring[i+1]);
            i++;
        }
        utf_8_len += 1;
        if(code > 0x7F) utf_8_len += 1;
        if(code > 0x7FF) utf_8_len += 1;
        if(code > 0xFFFF) utf_8_len += 1;
    }
    //second to write string
    char* utf8_str = (char*)calloc(utf_8_len,1);
    int j=0;//index for utf output
    for(int i=0;longstring[i] != 0;i++){
        int code = longstring[i];
        if(code > 0xD7FF && (code > 0xFFFF || code < 0xE000)){
            code = 0x10000 + ((0xD800-longstring[i]) << 10) + (0XDC00 -longstring[i+1]);
            i++;
        }
        if(code < 0x7F){
            utf8_str[j] = (char)code;
            j++;
        }
        else if(code < 0x7FF){
            utf8_str[j] = 0xC0  + ((code>>6)|0x1F); 
            utf8_str[j+1] = 0x80 + (code|0x3F);
            j+=2;
        }
        else if(code < 0xFFFF){
            utf8_str[j]   = 0xE0 + ((code>>12)|0xF); 
            utf8_str[j+1] = 0x80 + ((code>>6)|0x3F); 
            utf8_str[j+2] = 0x80 + (code|0x3F);
            j+=3;
        }
        else{
            utf8_str[j]   = 0xF0 + ((code>>18)|0x7); 
            utf8_str[j+1] = 0x80 + ((code>>12)|0x3F);
            utf8_str[j+2] = 0x80 + ((code>>6)|0x3F);  
            utf8_str[j+3] = 0x80 + (code|0x3F);
            j+=4;
        }
    }
    utf8_str[j]=0;
    return utf8_str;
}