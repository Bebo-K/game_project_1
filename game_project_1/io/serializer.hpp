#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/types/transform.hpp>

class Serializer{
    bool allocated;
    public:
    int data_length;
    int place;
    byte* raw_data;

    Serializer();
    Serializer(int size);
    Serializer(byte* existing_bytestream,int len);
    ~Serializer();

    void Allocate(int size);

    void PutByte(byte b);
    void PutInt(int i);
    void PutFloat(float f);
    void PutVec3(vec3 v);
    void PutQuaternion(quaternion q);
    void PutString(char* str);
    void PutWString(wchar* str);
    void WriteTransform(Transform* t);
    void WriteBytes(byte* dat,int len);
    byte* Bytes();
};

class Deserializer{
    public:
    int place;
    byte* raw_data;

    Deserializer(byte* data);

    byte GetByte();
    int GetInt();
    float GetFloat();
    vec3 GetVec3();
    quaternion GetQuaternion();
    char* GetString();
    wchar* GetWString();
    void ReadTransform(Transform* t);
    byte* CopyBytes(int bytes);
    void CopyBytesTo(byte* dest,int bytes);
    int  AmountRead();

};








#endif