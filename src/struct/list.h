#ifndef LIST_H
#define LIST_H

#include  <stdlib.h>
#include "data_types.h"


template <typename T>
class List;

template <typename T>
struct ListIterator{
    List<T>*    parent;
    int         index;
    T* operator*();
    ListIterator<T> operator++();
    bool operator==(ListIterator<T>& l2);
    bool operator!=(ListIterator<T>& l2);
};

//Combines DataArray with template programming and iterators to make data access easy.
//Note that Lists manage the lifecycle of all of it's entries (e.g. deconstructor is called on removed entries)
template <typename T>
class List{
    protected:
    DataArray data;
    int count;
    
    friend class ListIterator<T>;
    public:

    List():data(1,sizeof(T)){
        count=0;
    }

    ~List(){
        for(int i=0;i<data.slots;i++){
            if(data.occupancy.Get(i)){
                T* obj = (T*)data.Get(i);
                obj->~T();
            }
        }
        data.Clear();
    }

    int Count(){return count;}

    int IndexOf(T* obj){
        for(int i=0; i<count;i++){
            if(obj == data.Get(i))return i ;
        }
        return -1;
    }

    //Override for T
    T* Add(){
        count++;
        return new (data.Add()) T();
    }

    void Add(T* obj){
        count++;
        data.Add(obj);
    }

    void Delete(T* obj){
        int index = IndexOf(obj);
        if(index >= 0 && index <= data.slots){
            obj->~T();
            data.Remove(index);
            count--;
        }
    }

    void Delete(int index){
        if(index >= 0 && index <= data.slots){
            T* obj = (T*)data.Get(index);
            if(obj != nullptr){
                obj->~T();
                data.Remove(index);
                count--;
            }
        }
    }

    T* operator[](int index){
        return (T*)data.Get(index);
    }
    
    int next(int start_index){
        int next=start_index;
        while( ++next < data.slots && !data.occupancy.Get(next));
        return next;
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
    
    ListIterator<T> begin(){ return {this,next(-1)};}
    ListIterator<T> end(){return {this,data.slots};}
};




template <typename T>
T* ListIterator<T>::operator*(){
    if(index < 0){
        return null;
    }
    return (*parent)[index];
}

template <typename T>
ListIterator<T> ListIterator<T>::operator++(){
    index = parent->next(index);
    return (*this);
}

template <typename T>
bool ListIterator<T>::operator==(ListIterator<T>& l2){
    return index ==l2.index;
}
template <typename T>
bool ListIterator<T>::operator!=(ListIterator<T>& l2){
    return !(index ==l2.index);
}

#endif