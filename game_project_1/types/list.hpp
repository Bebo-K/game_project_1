#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <game_project_1/io/log.hpp>


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

//A dynamic array of object pointers (Not object memory slots like Array<T>). 
//Memory for pointers is not managed but ptrs are cleaned up by dtor()
template <typename T> 
class List{
    protected:
    T**     data;
    int     count;
    
    friend class ListIterator<T>;
    public:
    int     length;

    List(){
        length=1;
        count=0;
        data=(T**)calloc(length,sizeof(T*));
    }
    List(int size){
        length=size;
        count=0;
        if(size > 0){
            data= (T**)calloc(length,sizeof(T*));
        }
        else{data = nullptr;}
    }
    ~List(){
        length=0;
        count=0;
        free(data);
        data=nullptr; 
    }
    int Add(T* object){
        int slot_to_add=0;
        while(slot_to_add < length && data[slot_to_add] != nullptr){slot_to_add++;}
        if(slot_to_add == length){
            Resize((length > 0)? length*2 : 1);
        }
        data[slot_to_add] = object;
        count++;
        return slot_to_add;
    }
    T* Remove(int index){
        if(index < 0 || index >= length){logger::exception("List::Remove -> Index %d is out of range.",index);}
        T* ret = data[index];
        data[index] = nullptr;
        count--;
        return ret;
    }
    int Remove(T* object){
        for(int i=0;i<length;i++){
            if(data[i] == object){
                data[i] = nullptr;
                count--;
                return i;
            }
        }
        return -1;
    }
    void Set(int index,T* dat){
        if(index < 0 || index >= length){logger::exception("List::Set -> Index %d is out of range.",index);}
        if(data[index]==nullptr)count++;
        data[index]=dat;
    }
    T* operator[](int index){
        if(index < 0 || index >= length){logger::exception("List::operator[] -> Index %d is out of range.",index);}
        return data[index];
    }
    int GetIndex(T* object){
        for(int i=0;i<length;i++){
            if(data[i] == object){return i;}
        }
        return -1;
    }
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < length && data[next]== nullptr);
        return next;
    }
    int Count(){
        return count;
    }
    void Resize(int newsize){
        if(newsize == 0){Clear();return;}
        T** newdata = (T**)calloc(newsize,sizeof(T*));
        int length_to_copy = (length < newsize)?length:newsize;
        for(int i=0;i<length_to_copy;i++){newdata[i] = data[i];}
        free(data);
        length=newsize;
        data=newdata;
    }
    void Clear(){
        if(data != nullptr){
            free(data); 
            data=nullptr;
        }
        length=0;
        count=0;
    }

    int Last(){
        int last=0;
        for(int i=0;i<length;i++){if(data[i] != nullptr){last=i;}}
        return last;
    }

    //range-for loop iterator methods
    ListIterator<T> begin(){ return {this,NextIndex(-1)};}
    ListIterator<T> end(){ return {this,length};}
};


template <typename T>
T* ListIterator<T>::operator*(){
    if(index < 0||index >= parent->length){logger::exception("ListIterator::Operator* -> Index %d is out of range.",index);}
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