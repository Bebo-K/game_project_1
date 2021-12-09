#ifndef ARRAYS_H
#define ARRAYS_H

#include "data_types.h"

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








#endif