#include <game_project_1/io/serializer.hpp>
#include <game_project_1/io/log.hpp>
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
void Serializer::PutByte(byte b){
    if( (int)(place + 1) > data_length){
        logger::exception("Serializer.PutByte(%d) writes past the end of the data buffer (pos:%d,len%d)",b,place,data_length);};
    raw_data[place] = b;
    place++;
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
void Serializer::PutVec3(vec3 v){
    if((int)(place + sizeof(vec3)) > data_length){
        logger::exception("Serializer.PutVec3(%f,%f,%f) writes past the end of the data buffer (pos:%d,len%d)",v.x,v.y,v.z,place,data_length);}
    vec3* ptr = (vec3*)&raw_data[place];
    *ptr =  v;
    place += sizeof(vec3);
}

void Serializer::PutQuaternion(quaternion v){
    if((int)(place + sizeof(quaternion)) > data_length){
        logger::exception("Serializer.PutQuaternion(%f,%f,%f) writes past the end of the data buffer (pos:%d,len%d)",v.x,v.y,v.z,place,data_length);}
    quaternion* ptr = (quaternion*)&raw_data[place];
    *ptr =  v;
    place += sizeof(quaternion);
}


void Serializer::PutString(char* str){
    char* ptr = (char*)&raw_data[place];
    int str_len = cstr::len(str);
    int write_len = (str_len > 0)? (str_len+1)*sizeof(char) : sizeof(char);    
    if(place + write_len > data_length){
        logger::exception("Serializer.PutString(%s) writes past the end of the data buffer (pos:%d,len%d)",str,place,data_length);}

    if(str_len > 0){ memcpy(ptr,str,write_len); }
    else{ptr[0] = (char)0;}
    place += write_len;
}
void Serializer::PutWString(wchar* str){
    wchar* ptr = (wchar*)&raw_data[place];
    int str_len = wstr::len(str);
    int write_len = (str_len+1)*sizeof(wchar);   
    if(place + write_len > data_length){
        logger::exception("Serializer.PutWString(%S) writes past the end of the data buffer (pos:%d,len%d)",str,place,data_length);}

    if(str_len > 0){ memcpy(ptr,str,write_len); }
    else{ptr[0] = (wchar)0;}
    place += write_len;
}
void Serializer::WriteTransform(Transform* t){
    PutVec3({t->x,t->y,t->z});
    PutQuaternion(t->rotation);
    PutVec3(t->scale);
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

byte Deserializer::GetByte(){
    place += 1;
    return raw_data[place-1]; 
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
vec3 Deserializer::GetVec3(){
    vec3* ret = (vec3*)&raw_data[place];
    place += sizeof(vec3);
    return *ret;
}
quaternion Deserializer::GetQuaternion(){
    quaternion* ret = (quaternion*)&raw_data[place];
    place += sizeof(quaternion);
    return *ret;
}
char* Deserializer::GetString(){
    char* str = cstr::new_copy((char*)&raw_data[place]);
    place += cstr::len(str)+1;
    return str;
}
wchar* Deserializer::GetWString(){
    wchar* str = wstr::new_copy((wchar*)&raw_data[place]);
    place += (wstr::len(str)+1)*sizeof(wchar);
    return str;
}
void Deserializer::ReadTransform(Transform* t){
    vec3 pos = GetVec3();
    t->x=pos.x;
    t->y=pos.y;
    t->z=pos.z;
    t->rotation = GetQuaternion();
    t->scale    = GetVec3();
}

byte* Deserializer::CopyBytes(int bytes){
    byte* ret = (byte*)calloc(bytes,1);
    memcpy(ret,&raw_data[place],bytes);
    place += bytes;
    return ret;
}

void Deserializer::CopyBytesTo(byte* dest,int bytes){
    memcpy(dest,&raw_data[place],bytes);
    place += bytes;
}

int Deserializer::AmountRead(){return place;}

