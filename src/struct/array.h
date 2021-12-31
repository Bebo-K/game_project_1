#ifndef ARRAY_H
#define ARRAY_H


#include <string.h>
#include "../log.h"


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

//An iterable array that allocates memory but does not track it's state. Can be resized manually.
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
        if(index < 0 || index >= size){logger::exception("Array::[]] -> Index %d is out of range.",index);return nullptr;}
        return data[index];
    }

    void Resize(int new_size){
        T* new_dat = calloc(new_size,sizeof(T));
        memcpy(new_dat,data,(new_size > length)? length*sizeof(T),new_size*sizeof(T));
        free(data);
        data = new_dat;
        length = new_size;
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