#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

typedef unsigned int uint32;
typedef unsigned char byte;

//A dynamic array of bits. Used by other maps to keep track of data slot occupancy.
struct BitArray{
    int bits;
    byte* data;

    BitArray();
    BitArray(int bit_count);
    ~BitArray();
    void Resize(int new_bit_count);
    void Initialize(int bit_count);

    void Print();
    int CountBitsSet();
    int CountBitsUnset();
    bool Get(int index);
    void Set(int index);
    void Unset(int index);
    bool Toggle(int index);//returns new value;
    void Clear();
};


//A dynamic object array.
//Memory is allocated for objects inside the array, if a new object is added with no available slots, the array's size will be doubled.
//Because objects can have any data, a separate, additional bit array is kept for vacancy info of data slots.
//Add() and Remove() must be used to set/clear vacancy bits. 
//Though Get(),GetArray() and the subscript operator([]) all are ways to access data, they handle vacant object slots differently
//   Using the subscript operator([]) on a data slot with an unset occupancy bit will throw an exception. Only use on known valid slots.
//   Accessing objects with Get() with an unset occupancy bit will return a null pointer. Use this to iterate through only valid objects.
//   Accssing objects with GetArray will return all allocated data. It's not reccomended, but can be used to access *any* slot.
struct DataArray{
    int slots;
    int slot_size;
    BitArray occupancy;
    byte* data;

    DataArray();
    DataArray(int object_size);
    DataArray(int count,int object_size);
    ~DataArray();

    void* Add();
    int Add(void* object);
    void Remove(int index);

    void* Get(int index);
    byte* GetArray();

    int Count();
    void Clear();
    void Resize(int new_count);
    void Initialize(int count,int object_size);
};

//A dynamic array of pointers to objects. 
//Memory allocation for objects are not done by this array, only pointers are stored.
//Add() will overwrite+return the first null pointer, or double the array size if there are none.
//Remove() simply nulls out the indexed pointer.
//Since any slot could contain a null pointer, null checks should be done before accessing data.
//Relative to the start of the array, there are no methods defined to move data,
//  so the integer index of an object should be a safe reference.
struct PointerArray{
    byte** data;
    int slots;
    
    PointerArray();
    PointerArray(int initial_size);
    ~PointerArray();

    int Add(void* object);
    void* Remove(int index);
    int Remove(void* object);
    void* Get(int index);

    int Count();
    void Resize(int new_count);
    void Clear();
};

//Associative Array: PointerArray with an integer/pointer key association. As close to a map as it gets.
// Uses both the PointerArray system and DataArray vacancy bit array system, so 0/null values are valid
//Memory allocation for keys(if any) and values are not done by this array, only pointers are stored.
//Add() will overwrite+return the first empty slot, or double the array size if there are none.
//Remove() simply frees the array's slot.
//Relative to the start of the array, there are no methods defined to move data,
//  so the integer index of an object should be a safe reference if the user does not move array data.

//BIG CAVEAT to this is that the array is only safe for as many spaces as min(sizeof(int),sizeof(byte*)).
//On most systems both are 4, but...

//ANOTHER BIG CAVEAT - Don't mix types of keys.
// It won't complain if you add a char* key and an int key, but good luck remembering which ones you can dereference.
/*
union u_associative_array_key{
    int intvalue;
    byte* ptrvalue;
};
struct AssociativeArray{
    u_associative_array_key* key_data;
    BitArray slot_is_filled;
    byte** value_data;
    int slots;
    
    AssociativeArray();
    AssociativeArray(int initial_size);
    ~AssociativeArray();

    bool Add(int key,byte* value);
    bool Add(byte* key,byte* object);
    byte* Remove(int key);
    byte* Remove(byte* key);
    byte* Get(int index);
    byte* Get(byte* index);
    int   IndexOf(u_associative_array_key key);
    //StrGet is a special form of get that assumes the compares the key as a cstring pointer.
    byte* StrGet(const char* index);
    byte* StrRemove(const char* index);

    int Count();
    void Resize(int new_count);
};
*/

struct IDMap{
    int*        keys;
    byte**      values;
    int         slots;
    BitArray    slot_is_filled;

    IDMap();
    IDMap(int initial_size);
    ~IDMap();
    
    bool Add(int id,byte* value);
    byte* Remove(int id);
    bool  Has(int id);
    byte* Get(int id);
    void  Clear();
};

struct StringMap{
    char**  keys;
    byte**  values;
    int         slots;

    StringMap();
    StringMap(int initial_size);
    ~StringMap();

    bool Add(char* name,byte* value);
    byte* Remove(char* name);
    byte* Get(char* name);
    bool  Has(char* name);
    void  Clear();


};

namespace cstr{
    char* new_copy(const char* old_string);
    char* lowercase_copy(const char* old_string);
    char* append(const char* str1, const char* str2);
    char* append(const char* str1, char seperator, const char* str2);
    bool compare(const char* str1, const char* str2);
    bool starts_with(const char* str, const char* start);

    char* utf16_to_utf8(const wchar_t* longstring);
    /*
    int integer_from_string(char* str,int index);
    char* write_integer_string(int a, char *str, int index);

    float float_from_string(char* str,int index);
    char* write_float_string(int a, char *str, int index);

    int bool_from_string(char* str,int index);
    char* write_bool_string(int a, char *str, int index);
    */
};



#endif

