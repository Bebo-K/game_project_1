#ifndef POOL_H
#define POOL_H

#include <stdlib.h>
#include <string.h>
#include <game_project_1/types/arrays.hpp>
#include <game_project_1/io/log.hpp>
#include <new>


template <typename T>
class Pool;

template <typename T>
struct PoolIterator{
    Pool<T>*    parent;
    int         index;
    T* operator*();
    PoolIterator<T> operator++();
    bool operator==(PoolIterator<T>& l2);
    bool operator!=(PoolIterator<T>& l2);
};

//A dynamic, memory managed array of objects.
template <typename T>
class Pool{
    protected:
    DynamicArray data;
    int count;
    
    friend class PoolIterator<T>;
    public:

    Pool():data(1,sizeof(T)){
        count=0;
    }

    Pool(int size):data(size,sizeof(T)){
        count=0;
    }

    ~Pool(){
        for(byte* b:data){
            T* obj = (T*)b;
            obj->~T();
        }
        data.Clear();
    }

    int Count(){return count;}

    int IndexOf(T* obj){return data.Index((byte*)obj);}

    //Returns a new initialized object
    T* Add(){
        count++;
        return new (data.Allocate()) T();
    }

    //Returns an uninitialized data block for manual initialization
    T* Allocate(){
        count++;
        return (T*)data.Allocate();
    }

    //void Add(T* obj) DO NOT USE THIS PARADIGM. Memory is allocted by Pool, use 'new (Pool.Allocate()) T(args...)' instead

    void Delete(T* obj){
        int index = IndexOf(obj);
        if(index < 0 || index >= data.slots){logger::exception("Pool::Delete -> Index %d is out of range.",index);return;}
        obj->~T();
        data.Remove(index);
        count--;
    }

    void Delete(int index){
        if(index < 0 || index >= data.slots){logger::exception("Pool::Delete -> Index %d is out of range.",index);return;}
        T* obj = (T*)data.Get(index);
        if(obj != nullptr){
            obj->~T();
            data.Remove(index);
            count--;
        }
    }

    T* operator[](int index){
        return (T*)data.Get(index);
    }
    
    int next(int start_index){
        return data.NextNonEmpty(start_index);
    }

    void Clear(){
        T* objs;
        for(int i=0;i<data.slots;i++){
            objs = (T*)data.Get(i);
            if(objs != null){
                objs->~T();
            }
        }
        data.Clear();
        count=0;
    }

    T* Harden(){
        T* ret = (T*)malloc(sizeof(T)*count);
        int array_slot=0;
        for(T* obj: this){
            memcpy(&ret[array_slot],obj,sizeof(T));
            array_slot++;
        }
        return ret;
    }
    
    PoolIterator<T> begin(){ return {this,next(-1)};}
    PoolIterator<T> end(){return {this,data.slots};}
};

template <typename T>
T* PoolIterator<T>::operator*(){
    if(index < 0){
        return null;
    }
    return (*parent)[index];
}

template <typename T>
PoolIterator<T> PoolIterator<T>::operator++(){
    index = parent->next(index);
    return (*this);
}

template <typename T>
bool PoolIterator<T>::operator==(PoolIterator<T>& l2){
    return index ==l2.index;
}
template <typename T>
bool PoolIterator<T>::operator!=(PoolIterator<T>& l2){
    return !(index ==l2.index);
}

#endif