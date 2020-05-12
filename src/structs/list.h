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
template <typename T>
class List{
    protected:
    DataArray data;
    int count;

    ListIterator<T> bad_index(){return {this,-1};}

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
        long ptr = (long)obj;
        long base = (long)data.data;
        long offset = ptr-base;
        return offset/sizeof(T);
    }

    //Override for T
    T* Add(){
        count++;
        return new (data.Add()) T();
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

    void Clear(){
        T* objs;
        for(int i=0;i<data.slots;i++){
            objs = (T*)data.Get(i);
            if(objs != null){
                objs->~T();
            }
        }
        data.Clear();
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

    ListIterator<T> begin(){
        int i;
        for(i=0;data.occupancy.Get(i)==false;i++){
            if(i >= data.slots){return bad_index();}
        }
        return {this,i};
    }

    ListIterator<T> next(int index){
        int i=index+1;
        while(data.occupancy.Get(i)==false){
            i++;
            if(i >= data.slots){return bad_index();}
        }
        return {this,i};
    }

    ListIterator<T> end(){
        int end;
        for(int i=count-1;i <= data.slots; i++){
            if(data.occupancy.Get(i)){end=i;}
        }
        return {this,end};
    }

};




template <typename T>
T* ListIterator<T>::operator*(){
    return (*parent)[index];
}

template <typename T>
ListIterator<T> ListIterator<T>::operator++(){
    return parent->next(index);
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