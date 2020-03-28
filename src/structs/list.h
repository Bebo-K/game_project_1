    #ifndef LIST_H
#define LIST_H

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
        for(i=index+1;data.slot_is_filled.Get(i)==false;i++){
            if(i >= data.slots){return -1;}
        }
        return i;
    }

    bool IsLast(int index){
        for(int i=index+1;data.slot_is_filled.Get(i)==false;i++){
            if(i >= data.slots){return true;}
        }
        return true;
    }

    int Count(){return count;}

    int IndexOf(T* obj){
        return (int)((obj - data.data))/sizeof(T);
    }

    T* New(){
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
};

#endif