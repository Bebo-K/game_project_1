#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <string.h>
#include <game_project_1/io/log.hpp>
#include <game_project_1/types/str.hpp>


template <typename K,typename V>
class Map;

template <typename K,typename V>
struct Tuple{
    K key;
    V value;
};

template <typename K>
bool CompareKey(K k1, K k2){return k1==k2;}
inline bool CompareKey(char* k1, char* k2){return cstr::compare(k1,k2);}

template <typename K>
bool KeyNull(K k){return k==0;}
inline bool KeyNull(char* k){return k==nullptr;}

template <typename K>
K Map_SetValue(K k){return k;}
inline char* Map_SetValue(char* k){return cstr::new_copy(k);}

template <typename K>
void Map_UnsetValue(K* k){memset(k,0,sizeof(K));}
inline void Map_UnsetValue(char** k){
    if((*k)==nullptr)return;
    else{free((*k));(*k) = nullptr;}
}

template <typename K,typename V>
struct MapIterator{
    Map<K,V>*    parent;
    int         index;
    Tuple<K,V> operator*();
    MapIterator<K,V> operator++();
    bool operator==(MapIterator<K,V>& l2);
    bool operator!=(MapIterator<K,V>& l2);
};

//A dynamic array of object pointers, accessable by an array of keys. 
//Key values must be unique and non-key based access outside of iteration is discouraged.
template <typename K,typename V>
class Map{
    protected:
    K*      keys;
    V*      values;
    int     slots;
    int     count;

    void UnsetKey(int slot){
        if(KeyNull(keys[slot]))return;
        Map_UnsetValue(&keys[slot]);
    }
    void SetKey(int slot,K value){keys[slot] = Map_SetValue(value);}
    
    friend class MapIterator<K,V>;
    public:

    Map(){
        slots=1;
        count=0;
        keys=(K*)calloc(slots,sizeof(K));
        values=(V*)calloc(slots,sizeof(V));
    }
    Map(int size){
        slots=size;
        count=0;
        keys=(K*)calloc(slots,sizeof(K));
        values= (V*)calloc(slots,sizeof(V));
    }
    ~Map(){
        for(int i=0;i< slots;i++){
            UnsetKey(i);
        }
        free(keys);keys=nullptr;
        free(values);values=nullptr;
    }
    V Get(K key){
        for(int i=0;i< slots;i++){
            if(KeyNull(keys[i]))continue;
            if(CompareKey(key,keys[i])){return values[i];}
        }
        return nullptr;
    }
    int MatchIndex(K key){
        for(int i=0;i< slots;i++){
            if(KeyNull(keys[i]))continue;
            if(CompareKey(key,keys[i])){return i;}
        }
        return -1;
    }
    K KeyAtIndex(int index){
        if(index < 0 || index >= slots){logger::exception("Map::KeyAtIndex: Trying to access a map key with an invalid index:%d",index);}
        return keys[index];
    }
    V ValueAtIndex(int index){
        if(index < 0 || index >= slots){logger::exception("Map::ValueAtIndex: Trying to access a map value with an invalid index:%d",index);}
        return values[index];
    }
    bool Has(K key){return Get(key)!=nullptr;}
    bool Add(K key,V value){
        if(Has(key))return false;
        if(KeyNull(key)){logger::exception("Map::Add: Invalid Key");}

        int slot_to_add=0;
        while(slot_to_add < slots && !KeyNull(keys[slot_to_add])){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        SetKey(slot_to_add,key);
        memcpy(&values[slot_to_add],&value,sizeof(V));
        count++;
        return slot_to_add;
    }
    void Remove(K key){
        for(int i=0;i< slots;i++){
            if(keys[i]==nullptr)continue;
            if(cstr::compare(key,keys[i])){
                UnsetKey(i);
                memset(&values[i],0,sizeof(V));
                count--;
            }
        }
    }
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < slots && KeyNull(keys[next]));
        return next;
    }
    int Count(){return count;}
    void Resize(int newsize){
        if(newsize == 0){Clear();return;}
        V* newvalues = (V*)calloc(newsize,sizeof(V));
        K* newkeys = (K*)calloc(newsize,sizeof(K));
        int slots_to_copy = (slots < newsize)?slots:newsize;
        memcpy(newkeys,keys,sizeof(K)*slots_to_copy);
        memcpy(newvalues,values,sizeof(V)*slots_to_copy);
        free(keys);
        free(values);
        slots=newsize;
        keys=newkeys;
        values=newvalues;
    }
    void Clear(){
        for(int i=0;i<slots;i++){
            UnsetKey(i);
            memset(&values[i],0,sizeof(V));
        }
        slots=0;
        count=0;
    }

    //range-for loop iterator methods
    MapIterator<K,V> begin(){ return {this,NextIndex(-1)};}
    MapIterator<K,V> end(){ return {this,slots};}
};


template <typename K,typename V>
Tuple<K,V> MapIterator<K,V>::operator*(){
    if(index < 0||index >= parent->slots){logger::exception("ListIterator::Operator* -> Index %d is out of range.",index);}
    Tuple<K,V> ret;
    ret.key = parent->keys[index];
    memcpy(&ret.value,&parent->values[index],sizeof(V));
    return ret;
}

template <typename K,typename V>
MapIterator<K,V> MapIterator<K,V>::operator++(){
    index = parent->NextIndex(index);
    return (*this);
}
template <typename K,typename V>
bool MapIterator<K,V>::operator==(MapIterator<K,V>& l2){
    return index ==l2.index;
}
template <typename K,typename V>
bool MapIterator<K,V>::operator!=(MapIterator<K,V>& l2){
    return !(index ==l2.index);
}

class IdMap{
    protected:
        char**  keys;
        int*    values;
        int     slots;
        int     count;

    void UnsetKey(int slot){
        if(keys[slot]==nullptr)return;
        free(keys[slot]);
        keys[slot]=nullptr;
    }
    friend class MapIterator<char*,int>;
    public:

    IdMap(){
        slots=1;
        count=0;
        keys=(char**)calloc(slots,sizeof(char*));
        values=(int*)calloc(slots,sizeof(int));
    }
    IdMap(int size){
        slots=size;
        count=0;
        keys=(char**)calloc(slots,sizeof(char*));
        values=(int*)calloc(slots,sizeof(int));
    }
    ~IdMap(){
        for(int i=0;i< slots;i++){
            UnsetKey(i);
        }
        free(keys);keys=nullptr;
        free(values);values=nullptr;
    }
    int Get(char* key){
        for(int i=0;i< slots;i++){
            if(KeyNull(keys[i]))continue;
            if(CompareKey(key,keys[i])){return values[i];}
        }
        return -1;
    }
    bool Has(char* key){
        for(int i=0;i< slots;i++){
            if(KeyNull(keys[i]))continue;
            if(CompareKey(key,keys[i])){return true;}
        }
        return false;
    }
    bool Add(char* key,int value){
        if(Has(key))return false;
        int slot_to_add=0;
        while(slot_to_add < slots && !KeyNull(keys[slot_to_add])){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        keys[slot_to_add] = cstr::new_copy(key);
        values[slot_to_add] = value;
        count++;
        return slot_to_add;
    }
    void Remove(char* key){
        for(int i=0;i< slots;i++){
            if(keys[i]==nullptr)continue;
            if(cstr::compare(key,keys[i])){
                UnsetKey(i);
                values[i]=-1;
                count--;
            }
        }
    }
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < slots && KeyNull(keys[next]));
        return next;
    }
    int Count(){return count;}
    void Resize(int newsize){
        if(newsize == 0){Clear();return;}
        int* newvalues = (int*)calloc(newsize,sizeof(int));
        char** newkeys = (char**)calloc(newsize,sizeof(char*));
        int slots_to_copy = (slots < newsize)?slots:newsize;
        memcpy(newkeys,keys,sizeof(char*)*slots_to_copy);
        memcpy(newvalues,values,sizeof(int)*slots_to_copy);
        free(keys);
        free(values);
        slots=newsize;
        keys=newkeys;
        values=newvalues;
    }
    void Clear(){
        for(int i=0;i<slots;i++){
            UnsetKey(i);
            values[i]=0;
        }
        slots=0;
        count=0;
    }
};

#endif