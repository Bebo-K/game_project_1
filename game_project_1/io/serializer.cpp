#include <game_project_1/io/serializer.hpp>
#include <game_project_1/log.hpp>
#include <string>



Serializer::Serializer(){
    allocated=false;
    data_length=0;
    place=0;
    raw_data=nullptr;
}
Serializer::Serializer(int size){
    place=0;
    Allocate(size);
}
Serializer::Serializer(byte* existing_bytestream,int len){
    allocated=false;
    data_length=len;
    place=0;
    raw_data=existing_bytestream;
}
Serializer::~Serializer(){
    if(raw_data != nullptr && allocated){
        free(raw_data);
    }
}
void Serializer::Allocate(int size){
    allocated=true;
    raw_data = (byte*)calloc(size,1);
    data_length = size;
}
void Serializer::PutInt(int i){
    if( (int)(place + sizeof(int)) > data_length){
        logger::exception("Serializer.PutInt(%d) writes past the end of the data buffer (pos:%d,len%d)",i,place,data_length);}
    int* ptr = (int*)&raw_data[place];
    *ptr =  i;
    place += sizeof(int);
}
void Serializer::PutFloat(float f){
    if((int)(place + sizeof(float)) > data_length){
        logger::exception("Serializer.PutFloat(%f) writes past the end of the data buffer (pos:%d,len%d)",f,place,data_length);}
    float* ptr = (float*)&raw_data[place];
    *ptr =  f;
    place += sizeof(float);
}
void Serializer::PutString(char* str){
    char* ptr = (char*)&raw_data[place];
    int len = cstr::len(str)+1;
    if(place + len > data_length){
        logger::exception("Serializer.PutString(%s) writes past the end of the data buffer (pos:%d,len%d)",str,place,data_length);}
    memcpy(ptr,str,len);
    place += len;
}
void Serializer::PutWString(wchar* str){
    wchar* ptr = (wchar*)&raw_data[place];
    int len = (wstr::len(str)+1)*sizeof(wchar);
    if(place + len > data_length){
        logger::exception("Serializer.PutWString(%S) writes past the end of the data buffer (pos:%d,len%d)",str,place,data_length);}
    memcpy(ptr,str,len);
    place += len;
}
void Serializer::WriteBytes(byte* dat,int len){
    if(place + len > data_length){
        logger::exception("Serializer.WriteBytes([%d bytes]) writes past the end of the data buffer (pos:%d,len%d)",len,place,data_length);}
    memcpy(&raw_data[place],dat,len);
    place += len;
}
byte* Serializer::Bytes(){return raw_data;}

Deserializer::Deserializer(byte* source){
    place=0;
    raw_data=source;
}
int Deserializer::GetInt(){
    int* ret = (int*)&raw_data[place];
    place += sizeof(int);
    return *ret;
}
float Deserializer::GetFloat(){
    float* ret = (float*)&raw_data[place];
    place += sizeof(float);
    return *ret;
}
char* Deserializer::GetString(){
    char* str = cstr::new_copy((char*)&raw_data[place]);
    place += cstr::len(str)+1;
    return str;
}
wchar* Deserializer::GetWString(){
    wchar* str = wstr::new_copy((wchar*)&raw_data[place]);
    place += wstr::len(str)+1;
    return str;
}

byte* Deserializer::CopyBytes(int bytes){
    byte* ret = (byte*)calloc(bytes,1);
    memcpy(ret,&raw_data[place],bytes);
    place += bytes;
    return ret;
}

int Deserializer::AmountRead(){return place;}

