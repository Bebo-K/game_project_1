#ifndef ARRAYS_H
#define ARRAYS_H

#include "data_types.h"
#include <string.h>
#include "../log.h"



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


//A simple iterable array that allocates memory but does not track it's state. Can be resized by calling Resize() manually.
template <typename T>
class Array;
template <typename T>
struct ArrayIterator{
    Array<T>*    parent;
    int         index;
    T* operator*();
    ArrayIterator<T> operator++();
    bool operator==(ArrayIterator<T>& l2);
    bool operator!=(ArrayIterator<T>& l2);
};
template <typename T>
class Array{
    protected:
    T* data;
    
    friend class ArrayIterator<T>;
    public:
    int length;

    Array(){
        data=nullptr;
        length=0;
    }

    Array(int size){
        data=calloc(size,sizeof(T));
        length=size;
    }

    ~Array(){
        free(data);
        data=nullptr;
        length=0;
    }

    T* operator[](int index){
        if(index < 0 || index >= length){logger::exception("Array::[] -> Index %d is out of range.",index);return nullptr;}
        return &data[index];
    }

    void Resize(int new_size){
        T* new_dat = (T*)calloc(new_size,sizeof(T));
        memcpy((void*)new_dat,(void*)data,(new_size > length)? length*sizeof(T) : new_size*sizeof(T));
        free(data);
        data = new_dat;
        length = new_size;
    }

    int IndexOf(T* obj){
        int index = obj-data;
        if(index < 0 || index >= length){logger::exception("Array::Index of -> Index %d is out of range.",index);return 0;}
        return index;
    }
    
    ArrayIterator<T> begin(){ return {this,0};}
    ArrayIterator<T> end(){return {this,length};}
};

template <typename T>
T* ArrayIterator<T>::operator*(){
    return (*parent)[index];
}

template <typename T>
ArrayIterator<T> ArrayIterator<T>::operator++(){
    index++;
    return (*this);
}

template <typename T>
bool ArrayIterator<T>::operator==(ArrayIterator<T>& l2){
    return index ==l2.index;
}
template <typename T>
bool ArrayIterator<T>::operator!=(ArrayIterator<T>& l2){
    return !(index ==l2.index);
}





#endif