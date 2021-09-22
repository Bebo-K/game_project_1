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


//A resizable array of individually settable bits.
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

//A resizeable array of data blocks and corresponding BitArray of block occupancy.
//Memory blocks can have any data.
//Allocated memory is doubled by default once the array runs out of slots.
//Add(),Allocate() and Remove() must be used to add/remove elements to maintain the vacancy bit array.
//Though Get(),GetArray() and the subscript operator([]) all are ways to access data, they handle vacant object slots differently
class DynamicArray;
struct DynamicArrayIterator{
    DynamicArray* parent;
    int          index;
    byte* operator*();
    DynamicArrayIterator operator++();
    bool operator==(DynamicArrayIterator& l2);
    bool operator!=(DynamicArrayIterator& l2);
};
class DynamicArray{
    private:
    BitArray occupancy;
    byte* data;
    
    friend class DynamicArrayIterator;
    public:
    int slots;
    int slot_size;

    //DynamicArray();
    DynamicArray(int block_size);
    DynamicArray(int count,int block_size);
    ~DynamicArray();

    void* Allocate();//returns empty space for one object. Example usage: 'Object* o = new (DynamicArray.Allocate()) Object()'
    int Add(void* object);//copies existing data into a new slot.
    void Remove(int index);//deallocates memory for one slot. This does not free any objects allocated with the data.

    int  Index(byte* data);//If data is a pointer inside this memory block, returns it's index in the array.
    void* Get(int index);
    int   NextNonEmpty(int index);

    int Count();
    void Clear();
    void Resize(int new_count);   
    DynamicArrayIterator begin();
    DynamicArrayIterator end();
};

//A pair of dynamic arrays, one of unique integer keys and another of byte* data pointers
//Similar to PointerArray, except objects are accessed by key rather than slot index
/*
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
*/

//Dynamic array pair for string -> byte* mapping,
/*
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
*/

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

