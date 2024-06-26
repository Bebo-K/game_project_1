#ifndef ARRAYS_H
#define ARRAYS_H

#include <string.h>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/types/iterator.h>


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

//Wrapper class for {T* array;int arraySize;}. Safe for objects with no trivial copy-assignment.
template <typename T>
struct Array{ 
    friend struct ArrayIterator<T>;
    int length;
    T* data; 

    Array(){
        data=nullptr;
        length=0;
    }

    Array(int size){
        data=nullptr;
        length=0;
        Init(size);
    }

    ~Array(){Destroy();}

    void Init(int size){
        if(length > 0){
            logger::exception("Array::Init -> Memory leak condition, array has already been allocated.");
        }
        if(size == 0){ 
            data=nullptr;
            length=0;
        }
        else{
            data= new T[size];
            length=size;
        }
    }

    void Destroy(){
        delete[] data;
        length=0;
        data=nullptr;
    };

    T* operator[](int index){
        if(index < 0 || index >= length){logger::exception("Array::[] -> Index %d is out of range.\n",index);return nullptr;}
        return &data[index];
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


//A resizable array of individually settable bits.
struct BitArray{
    int bits;
    int allocated;
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

    int GetSizeInBytes();
    int GetSizeInInts();
};

//A resizeable array of unique integers. Order is not guaranteed as things may shift 
class IntegerSet{
    private:
    int     allocated;
    public:
    int     length;
    int*    data;

    IntegerSet();
    IntegerSet(int length);
    ~IntegerSet();

    void Add(int entry);
    bool Has(int entry);
    void Remove(int entry);
    void Clear();


    int operator[](int index);
    //int* Harden();
};

//A resizeable array of data blocks and corresponding BitArray of block occupancy.
//Memory blocks can have any data.
//Allocated memory is doubled by default once the array runs out of slots.
//Add(),Allocate() and Remove() must be used to add/remove elements to maintain the vacancy bit array.
//Though Get(),GetArray() and the subscript operator([]) all are ways to access data, they handle vacant object slots differently
class DynamicArray;
typedef Iterator<DynamicArray,byte*> DynamicArrayIterator;

class DynamicArray{
    private:
    BitArray occupancy;
    byte* data;
    
    friend DynamicArrayIterator;
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

template<>
byte* DynamicArrayIterator::GetResult();

template<>
int DynamicArrayIterator::Next();

#endif