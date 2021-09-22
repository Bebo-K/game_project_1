
#include "data_types.h"
#include "../log.h"
#include <stdlib.h>
#include <string.h>

bool GET_BIT(int x,int bit){return ((x & (1 << bit)) != 0);}

void* i_to_p(int ptr){
    byte* ret = null;
    ret += ptr;
    return ret;
}

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


/*
DynamicArray::DynamicArray():occupancy(){
    slots=0;
    slot_size=0;
    data=null;
}
DynamicArray::DynamicArray(int object_size):occupancy(1){
    slots=1;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
}
DynamicArray::DynamicArray(int count,int object_size):occupancy(count){
    slots=count;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
}
DynamicArray::~DynamicArray(){
    free(data);
}
void* DynamicArray::Add(){
}
int DynamicArray::Add(void* object){
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
void DynamicArray::Remove(int index){
    if(index < 0 || index >= slots){logger::exception("DynamicArray::Remove -> Index %d is out of range.",index);}
    occupancy.Unset(index);
    memset ((char*)data+(index*slot_size),0,slot_size);
}
void* DynamicArray::Get(int index){
    if(index < 0 || index >= slots){logger::exception("DynamicArray::Get -> Index %d is out of range.",index);}
    if(!occupancy.Get(index)){return null;}
    return (char*)data+(index*slot_size);
}
byte* DynamicArray::GetArray(){
    return data;
}
int DynamicArray::Count(){
    return occupancy.CountBitsSet();
}
void DynamicArray::Resize(int new_count){
    occupancy.Resize(new_count);
    byte* new_data = (byte*)calloc(new_count,slot_size);
    if(slots <= new_count){
        memcpy(new_data,data,slots*slot_size);
    }
    slots = new_count;
    free(data);
    data = new_data;
}
void DynamicArray::Clear(){
    memset(data,0,slot_size*slots);
    occupancy.Clear();
}
void DynamicArray::Initialize(int count,int object_size){
    slots=count;
    slot_size=object_size;
    data = (byte*)calloc(slots,slot_size);
    occupancy.Initialize(count);
}

*/
//////////////////////////
///////***IDMap***///////
////////////////////////
/*
IDMap::IDMap():slot_is_filled(DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE){
    slots=DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE;
    keys = (int*)calloc(DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE,sizeof(int));
    values = (byte**)calloc(DEFAULT_DYNAMIC_ARRAY_INITIAL_SIZE,sizeof(byte*));
}
IDMap::IDMap(int initial_size):slot_is_filled(initial_size){
    slots=initial_size;
    keys = (int*)calloc(initial_size,sizeof(int));
    values = (byte**)calloc(initial_size,sizeof(byte*));
}
IDMap::~IDMap(){
    free(keys);keys=null;
    free(values);values=null;
}
bool IDMap::Add(int id,byte* data){
    if(Has(id))return false;

    for(int i=0;i< slots;i++){
        if(slot_is_filled.Get(i)==false){
            slot_is_filled.Set(i);
            keys[i] = id;
            values[i] = data;
            return true;
        }
    }
    
    int* double_keys = (int*)calloc(slots*2,sizeof(int));
    byte** double_values = (byte**)calloc(slots*2,sizeof(byte*));
        memcpy(double_keys,keys,sizeof(int)*slots);
        memcpy(double_values,values,sizeof(byte*)*slots);
        free(keys);keys=double_keys;
        free(values);values=double_values;
        slot_is_filled.Resize(slots*2);
    
    slot_is_filled.Set(slots);
    keys[slots] = id;
    values[slots] = data;
    slots *= 2;
    return true;
}

byte* IDMap::Remove(int id){
    for(int i=0;i< slots;i++){
        if(!slot_is_filled.Get(i))continue;
        if(id == keys[i]){
            slot_is_filled.Unset(i);
            return values[i];
        }
    }
    return null;
}

byte* IDMap::Get(int id){
    for(int i=0;i< slots;i++){
        if(!slot_is_filled.Get(i))continue;
        if(id == keys[i]){return values[i];}
    }
    return null;
}

bool IDMap::Has(int id){
    for(int i=0;i< slots;i++){
        if(!slot_is_filled.Get(i))continue;
        if(id == keys[i]){return true;}
    }
    return false;
}

void IDMap::Clear(){
    for(int i=0;i<slots;i++){
        keys[i] = 0;
        values[i] = null;
    }
    slot_is_filled.Clear();
}

StringMap::StringMap(){
    slots=1;
    keys = (char**)calloc(slots,sizeof(char*));
    values = (byte**)calloc(slots,sizeof(byte*));
}
StringMap::StringMap(int initial_size){
    slots=initial_size;
    keys = (char**)calloc(initial_size,sizeof(char*));
    values = (byte**)calloc(initial_size,sizeof(byte*));
}
StringMap::~StringMap(){
    for(int i=0;i< slots;i++){
        if(keys[i]==null)continue;
        free(keys[i]);keys[i]=null;
    }
    free(keys);keys=null;
    free(values);values=null;
}
bool StringMap::Add(char* id,byte* data){
    if(Has(id))return false;

    for(int i=0;i< slots;i++){
        if(keys[i]==null){
            keys[i] = cstr::new_copy(id);
            values[i] = data;
            return true;
        }
    }
    
    char** double_keys = (char**)calloc(slots*2,sizeof(char*));
    byte** double_values = (byte**)calloc(slots*2,sizeof(byte*));
        memcpy(double_keys,keys,sizeof(char*)*slots);
        memcpy(double_values,values,sizeof(byte*)*slots);
        free(keys);keys=double_keys;
        free(values);values=double_values;
    
    keys[slots] = cstr::new_copy(id);
    values[slots] = data;
    slots *= 2;
    return true;
}

byte* StringMap::Remove(char* id){
    for(int i=0;i< slots;i++){
        if(keys[i]==null)continue;
        if(cstr::compare(id,keys[i])){
            free(keys[i]);keys[i]=null;
            return values[i];
        }
    }
    return null;
}

byte* StringMap::Get(char* id){
    for(int i=0;i< slots;i++){
        if(keys[i]==null)continue;
        if(cstr::compare(id,keys[i])){return values[i];}
    }
    return null;
}

bool StringMap::Has(char* id){
    for(int i=0;i< slots;i++){
        if(keys[i]==null)continue;
        if(cstr::compare(id,keys[i])){return true;}
    }
    return false;
}

void StringMap::Clear(){
    for(int i=0;i<slots;i++){
        keys[i] = null;
        values[i] = null;
    }
}

int    StringMap::Max(){return slots;}
byte*   StringMap::At(int index){if(keys[index]!= null)return values[index];else return null;}

*/

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
bool cstr::contains(const char* str, const char* substr){
    int i,j;
    for(i=0;str[i] != 0;i++){
        j=0;
        for(j=0;str[i+j]==substr[j];j++){
            if(substr[j]==0){
                return true;
                }
        }
    }
    return false;
}
char* cstr::substr_before(char* str,char separator){
    int sep_pos =0;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;break;}}
    int new_strlen = sep_pos-1;
    if(new_strlen==0)return nullptr;
    char* result= (char*)calloc(new_strlen+1,sizeof(char));
    for(int i=0;i<sep_pos;i++){
        result[i]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
char* cstr::substr_after(char* str,char separator){
    int sep_pos =0;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;break;}}
    int new_strlen = cstr::len(str)-sep_pos;
    if(new_strlen==0)return nullptr;
    char* result= (char*)calloc(new_strlen+1,sizeof(char));
    for(int i=sep_pos;str[i]!=0;i++){
        result[i-sep_pos]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
int cstr::len(char* str){
    int i;
    for(i=0;str[i]!=0;i++);
    return i;
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
    char* utf8_str = (char*)calloc(utf_8_len,sizeof(char));
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
int cstr::integer_from_string(char* str){
    int value=0;
    for(int index=0;str[index]!=0;index++){
        if(str[index] >= '0' && str[index] <= '9'){
            value *= 10;
            value += str[index]-'0';
        }
        else return -1;
    }
    return value;
}
char* cstr::write_integer_string(int a){
    int len=0;
    for(int val=a;val!=0;len++){
        val = val/10;
    }
    char* ret = (char*)malloc(len+1);
    int ones=0;
    int val = a;
    for(int i=0;i<len;i++){
        ones = val%10;
        ret[(len-1)-i]=ones+'0';
        val/= 10;
    }
    
    ret[len]=0;
    return ret;
}
float cstr::float_from_string(char* str){
    return (float)atof(str);
}
char* cstr::write_float_string(float a){
    int before_decimal_places = 1;
    int after_decimal_places=8;
    int sign_places = (a<0)? 1:0;

    for(float decimal=10;a > decimal; a *= 10){before_decimal_places++;}

    char* ret = new char[sign_places+before_decimal_places+after_decimal_places+2];
    sprintf(ret,"%8f",a);
    return ret;
}

bool cstr::bool_from_string(char* str){
    if(cstr::compare(str,"true"))return true;
    if(cstr::compare(str,"false"))return false;
    return false;
}
char* cstr::write_bool_string(bool val){
    return cstr::new_copy(val?"true":"false");
}

text_char* TextString::from_cstr(char* str){
    if(str==null)return null;
    int i;
    for(i=0;str[i]!=0;i++);
    text_char* ret = new text_char[i+1];
    for(i=0;str[i]!=0;i++){ret[i] = (int)str[i];}
    ret[i]=0;
    return ret;
}
int TextString::length(text_char* str){
    if(str == nullptr)return -1;
    int ret;
    for(ret=0;str[ret]!= 0;ret++);
    return ret;
}
int TextString::write(char* str,text_char* dest){
    int ret;
    for(ret=0;str[ret] != 0;ret++){
        dest[ret] = (text_char)str[ret];
    }
    dest[ret]=0;
    return ret;
}
text_char* TextString::first(text_char* src,int max){
    int len = length(src);
    len = (max<len)?max:len;
    text_char* ret = (text_char*)calloc(len,sizeof(text_char));
    for(int i=0;src[i]!=0 && i<len;i++){ret[i]=src[i];}
    return ret;
}
text_char* TextString::substr(text_char* src,int start,int len){
    int strlen = length(src);
    if((start+strlen) >= len || start < 0){logger::exception("Invalid substring indices: %d to %d for string %s",start,start+strlen,src);return nullptr;}
    text_char* ret = (text_char*)calloc(len,sizeof(text_char));
    for(int i=start;src[i]!=0 && i<(start+len);i++){ret[i]=src[i];}
    return ret;
}
text_char* TextString::concat(text_char* a_part,text_char* b_part){
    text_char* result= (text_char*)calloc(length(a_part)+length(b_part)+1,sizeof(text_char));
    int i;
    for(i=0;a_part[i]!=0;i++){result[i]=a_part[i];}
    int a_len=i;
    for(i=0;b_part[i]!=0;i++){result[a_len+i]=b_part[i];}
    result[a_len+i]=0;
    return result;
}
text_char* TextString::copy(text_char* str){
    int strlen = length(str);
    text_char* result= (text_char*)calloc(strlen+1,sizeof(text_char));
    for(int i=0;str[i]!=0;i++){result[i]=str[i];}
    result[strlen]=0;
    return result;
}
text_char* TextString::substr_after(text_char* str,unsigned int separator){
    int sep_pos =0;
    for(int i=0;str[i]!=0;i++){if(str[i]==separator){sep_pos=i+1;break;}}
    int new_strlen = length(str)-sep_pos;
    if(new_strlen==0)return nullptr;
    text_char* result= (text_char*)calloc(new_strlen+1,sizeof(text_char));
    for(int i=sep_pos;str[i]!=0;i++){
        result[i-sep_pos]=str[i];
    }
    result[new_strlen]=0;
    return result;
}
