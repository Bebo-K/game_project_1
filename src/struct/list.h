#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include "../log.h"


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

//A dynamic array of object pointers. Memory for objects is not managed.
template <typename T>
class List{
    protected:
    T**     data;
    int     slots;
    int     count;
    
    friend class ListIterator<T>;
    public:

    List(){
        slots=1;
        count=0;
        data=(T**)calloc(slots,sizeof(T*));
    }
    List(int size){
        slots=size;
        count=0;
        data= (T**)calloc(slots,sizeof(T*));
    }
    ~List(){
        free(data); 
    }
    int Add(T* object){
        int slot_to_add=0;
        while(slot_to_add < slots && data[slot_to_add] != nullptr){slot_to_add++;}
        if(slot_to_add == slots){
            Resize(slots*2);
        }
        data[slot_to_add] = object;
        count++;
        return slot_to_add;
    }
    T* Remove(int index){
        if(index < 0 || index >= slots){logger::exception("List::Remove -> Index %d is out of range.",index);}
        T* ret = data[index];
        data[index] = nullptr;
        count--;
        return ret;
    }
    int Remove(T* object){
        for(int i=0;i<slots;i++){
            if(data[i] == object){
                data[i] = nullptr;
                count--;
                return i;
            }
        }
        return -1;
    }
    T* operator[](int index){
        if(index < 0 || index >= slots){logger::exception("List::operator[] -> Index %d is out of range.",index);}
        return data[index];
    }
    int GetIndex(T* object){
        for(int i=0;i<slots;i++){
            if(data[i] == object){return i;}
        }
    }
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < slots && data[next]== nullptr);
        return next;
    }
    int Count(){return count;}
    void Resize(int newsize){
        if(newsize == 0){Clear();return;}
        T** newdata = (T**)calloc(newsize,sizeof(T*));
        int slots_to_copy = (slots < newsize)?slots:newsize;
        for(int i=0;i<slots_to_copy;i++){newdata[i] = data[i];}
        free(data);
        slots=newsize;
        data=newdata;
    }
    void Clear(){
        if(data != nullptr){
            free(data); 
            data=nullptr;
        }
        slots=0;
        count=0;
    }

    //range-for loop iterator methods
    ListIterator<T> begin(){ return {this,NextIndex(-1)};}
    ListIterator<T> end(){ return {this,slots};}
};


template <typename T>
T* ListIterator<T>::operator*(){
    if(index < 0||index >= parent->slots){logger::exception("ListIterator::Operator* -> Index %d is out of range.",index);}
    return parent->data[index];
}
template <typename T>
ListIterator<T> ListIterator<T>::operator++(){
    index = parent->NextIndex(index);
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