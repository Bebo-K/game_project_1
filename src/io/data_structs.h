#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

struct BitArray{
    int bits;
    unsigned char* data;

    BitArray();
    BitArray(int bit_count);
    ~BitArray();
    void Resize(int new_bit_count);

    void Print();
    int CountBitsSet();
    int CountBitsUnset();
    bool Get(int index);
    void Set(int index);
    void Unset(int index);
    bool Toggle(int index);//returns new value;
};

//A dynamic object array.
//Memory is allocated for objects inside the array, if a new object is added with no available slots, the array's size will be doubled.
//Because objects can have any data, a separate, additional bit array is kept for vacancy info of data slots.
//Add() and Remove() must be used to set/clear vacancy bits. 
//Though Get(),GetArray() and the subscript operator([]) all are ways to access data, they handle vacant object slots differently
//   Using the subscript operator([]) on a data slot with an unset occupancy bit will throw an exception. Only use on known valid slots.
//   Accessing objects with Get() with an unset occupancy bit will return a null pointer. Use this to iterate through only valid objects.
//   Accssing objects with GetArray will return all allocated data. It's not reccomended, but can be used to access *any* slot.
template <typename T>
struct DataArray{
    int slots;
    int slot_size;
    BitArray slot_is_filled;
    void* data;

    DataArray(int object_size);
    DataArray(int count,int object_size);

    int Add();
    int Add(T* object);
    void Remove(int index);

    T &operator [](int);
    T* Get(int index);
    T* GetArray();

    int Count();
    void Resize(int new_count);
};

//A dynamic array of pointers to objects. 
//Memory allocation for objects are not done by this array, only pointers are stored.
//Add() will overwrite+return the first null pointer, or double the array size if there are none.
//Remove() simply nulls out the indexed pointer.
//Since any slot could contain a null pointer, 
template <typename T>
struct PointerArray{
    T** data;
    int slots;
    
    PointerArray(int initial_size);
    PointerArray();

    int Add(T* object);
    T* Remove(int index);
    T* Get(int index);

    int Count();
    void Resize(int new_count);
};

namespace str{
    int integer_from_string(char* str,int index);
    char* write_integer_string(int a, char *str, int index);

    float float_from_string(char* str,int index);
    char* write_float_string(int a, char *str, int index);

    int bool_from_string(char* str,int index);
    char* write_bool_string(int a, char *str, int index);
};


#endif