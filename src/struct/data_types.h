#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#ifndef null
#define null nullptr
#endif

#define SET_BIT(x,bit) x |= (1<<(bit))
#define CLEAR_BIT(x, bit) x &= ~(1<<(bit))

typedef unsigned int uint32;
typedef unsigned char byte;
typedef unsigned int text_char;

void* i_to_p(int ptr);
bool GET_BIT(int x,int bit);


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

//A pair of dynamic arrays, one of unique integer keys and another of byte* data pointers
//Similar to PointerArray, except objects are accessed by key rather than slot index
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

//Dynamic array pair for string -> byte* mapping,
struct StringMap{
    char**  keys;
    byte**  values;
    int     slots;

    StringMap();
    StringMap(int initial_size);
    ~StringMap();

    bool  Add(char* name,byte* value);
    byte* Remove(char* name);
    byte* Get(char* name);
    bool  Has(char* name);
    void  Clear();

    int    Max();
    byte*  At(int index);
};

namespace cstr{
    char* new_copy(const char* old_string);
    char* lowercase_copy(const char* old_string);
    char* append(const char* str1, const char* str2);
    char* append(const char* str1, char seperator, const char* str2);
    bool compare(const char* str1, const char* str2);
    bool starts_with(const char* str, const char* start);
    bool contains(const char* str, const char* substr);
    char* substr_after(char* str,char separator);//returns the substring after the first occurance of the separator char
    char* substr_before(char* str,char separator);//returns the substring after the first occurance of the separator char
    int len(char* str);

    char* utf16_to_utf8(const wchar_t* longstring);
    
    int integer_from_string(char* str);
    char* write_integer_string(int a);
    
    float float_from_string(char* str);
    char* write_float_string(float a);
    
    bool bool_from_string(char* str);
    char* write_bool_string(bool a);
    
};

namespace TextString{
    text_char* from_cstr(char* str);
    int length(text_char* str);
    int write(char* str,text_char* dest);
    
    text_char* first(text_char* src,int max);
    text_char* substr(text_char* src,int start,int end);
    text_char* concat(text_char* a_part,text_char* b_part);
    text_char* copy(text_char* str);

    //Gets the substring of a text_char string after the occurance of a seperator char
    //1.the returned string will not have the separator char at the beginning
    //2.if the separator char isn't found, a copy of the original string is returned.
    //3.if the separator char is at the very end of the string, nullptr is returned.
    text_char* substr_after(text_char* str,unsigned int separator);

};

#endif

