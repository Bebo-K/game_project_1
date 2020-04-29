#ifndef LIST_H
#define LIST_H

#include  <stdlib.h>
#include "data_types.h"

template <typename T>
class List{
    DataArray data;
    int count;
    public:

    List():data(2){
        count=0;
    }

    int Begin(){
        int i;
        for(i=0;data.slot_is_filled.Get(i)==false;i++){
            if(i >= data.slots){return -1;}
        }
        return i;
    }

    int Next(int index){
        int i;
        if(index+1 >= data.slots)return -1;
        for(i=index+1;i< data.slots ; i++){
            if(data.slot_is_filled.Get(i))return i;
        }
        return -1;
    }

    bool Has(int index){
        if(index >= count || index < 0)return false;
        return data.slot_is_filled.Get(index);
    }

    int Count(){return count;}

    int IndexOf(T* obj){
        return (int)((obj - data.data))/sizeof(T);
    }

    T* New(){
        count++;
        return new (data.Add()) T();
    }

    void Put(T* obj){
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
        for(int i=Begin();Has(i);i=Next(i)){
            memcpy(&ret[array_slot],data.Get(i),sizeof(T));
        }

        return ret;
    }

};

#endif