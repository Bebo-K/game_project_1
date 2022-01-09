#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "../struct/data_types.h"
#include "../struct/str.h"

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

    void PutInt(int i);
    void PutFloat(float f);
    void PutString(char* str);
    void PutWString(wchar* str);
    void WriteBytes(byte* dat,int len);
    byte* Bytes();
};

class Deserializer{
    public:
    int place;
    byte* raw_data;

    Deserializer(byte* data);

    int GetInt();
    float GetFloat();
    char* GetString();
    wchar* GetWString();
    byte* CopyBytes(int bytes);
    int  AmountRead();

};










#endif