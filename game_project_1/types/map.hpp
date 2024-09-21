#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <string.h>
#include <game_project_1/types/primitives.hpp>
#include <game_project_1/io/log.hpp>
#include <game_project_1/types/str.hpp>


template <typename K,typename V>
class Map;
template <typename K,typename V>
class Dictionary;

template <typename K,typename V>
struct Tuple{
    K key;
    V value;
};

namespace MapUtils{
    template <typename K>
    bool Compare(K k1, K k2){return k1==k2;}
    bool Compare(char* k1, char* k2);

    template <typename K>
    bool IsNull(K k){return k==0;}
    bool IsNull(char* k);

    template <typename K>
    K NullValue(K k){return nullptr;}
    int NullValue(int k);

    template <typename T>
    void Copy(T& t,T* dest){new(dest) T(t);}
    void Copy(char*& t,char** dest);
    void Copy(int& t,int* dest);

    template <typename T>
    void Set(T* t,T* dest){*dest = *t;}

    template <typename T>
    void Unset(T* t){t->~T();}
    void Unset(char** t);
    void Unset(int* t);

    template <typename K,typename V>
    struct MapIterator{
        Map<K,V>*    parent;
        int         index;
        Tuple<K,V*> operator*();
        MapIterator<K,V> operator++();
        bool operator==(MapIterator<K,V>& l2);
        bool operator!=(MapIterator<K,V>& l2);
    };

    template <typename K,typename V>
    struct DictionaryIterator{
        Dictionary<K,V>* parent;
        int         index;
        Tuple<K,V> operator*();
        DictionaryIterator<K,V> operator++();
        bool operator==(DictionaryIterator<K,V>& l2);
        bool operator!=(DictionaryIterator<K,V>& l2);
    };
}

//Basic memory-managed associative array. 
//keys (K) should be trivially copyable- only copies of key objects can be retrieved from the map
//values (V) are considered as "owned" by the map, meaning references to the object stored on the map (V*) are retrieved and not copies.
//For maps of trivially-copyable values (e.g. Map<Int,char*>), use Dictionary<K,V> instead
template <typename K,typename V>
class Map{
    protected:
    K*      keys;
    V*      values;
    int     slots;
    int     count;

    void UnsetEntry(int slot){
        if(MapUtils::IsNull(keys[slot]))return;
        MapUtils::Unset(&keys[slot]);
        MapUtils::Unset(&values[slot]);
        keys[slot]=MapUtils::NullValue(keys[slot]);
    }
    void SetKey(int slot,K key){MapUtils::Copy(key,&keys[slot]);}
    void SetValue(int slot,V value){MapUtils::Set(value,&values[slot]);}
    
    friend class MapUtils::MapIterator<K,V>;
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
            if(MapUtils::IsNull(keys[i]))continue;
            UnsetEntry(i); 
        }
        free(keys);keys=nullptr;
        free(values);values=nullptr;
    }
    V* Get(K key){
        for(int i=0;i< slots;i++){
            if(MapUtils::IsNull(keys[i]))continue;
            if(MapUtils::Compare(key,keys[i])){return &values[i];}
        }
        return nullptr;
    }
    bool Has(K key){return Get(key)!=nullptr;}
    V* Add(K key){
        if(Has(key))return Get(key);
        if(MapUtils::IsNull(key)){logger::exception("Map::Add: Invalid Key");}
        int slot_to_add=0;
        while(slot_to_add < slots && !MapUtils::IsNull(keys[slot_to_add])){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        SetKey(slot_to_add,key);
        count++;
        return &values[slot_to_add];
    }
    void Remove(K key){
        for(int i=0;i< slots;i++){
            if(MapUtils::IsNull(keys[i]))continue;
            if(MapUtils::Compare(key,keys[i])){
                UnsetEntry(i);
                return;
            }
        }
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
            UnsetEntry(i);
        }
        slots=0;
        count=0;
    }

    //range-for loop iterator methods
    MapUtils::MapIterator<K,V> begin(){ return {this,NextIndex(-1)};}
    MapUtils::MapIterator<K,V> end(){ return {this,slots};}

    private:
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < slots && MapUtils::IsNull(keys[next]));
        return next;
    }
};


template <typename K,typename V>
Tuple<K,V*> MapUtils::MapIterator<K,V>::operator*(){
    if(index < 0||index >= parent->slots){logger::exception("ListIterator::Operator* -> Index %d is out of range.",index);}
    return {parent->keys[index], &parent->values[index]};
}
template <typename K,typename V>
MapUtils::MapIterator<K,V> MapUtils::MapIterator<K,V>::operator++(){
    this->index=parent->NextIndex(index);
    return *this;
}
template <typename K,typename V>
bool MapUtils::MapIterator<K,V>::operator==(MapIterator<K,V>& l2){
    return index ==l2.index;
}
template <typename K,typename V>
bool MapUtils::MapIterator<K,V>::operator!=(MapIterator<K,V>& l2){
    return !(index ==l2.index);
}



//Unmodifyable associative array. Once stored, only copies of keys and values can be retrieved
template <typename K,typename V>
class Dictionary{
    protected:
    K*      keys;
    V*      values;
    int     slots;
    int     count;

    void UnsetEntry(int slot){
        if(MapUtils::IsNull(keys[slot]))return;
        MapUtils::Unset(&keys[slot]);
        memset(&keys[slot],0,sizeof(K));
        MapUtils::Unset(&values[slot]);
        memset(&keys[slot],0,sizeof(K));
    }
    void SetKey(int slot,K key){MapUtils::Copy(key,&keys[slot]);}
    void SetValue(int slot,V value){MapUtils::Copy(value,&values[slot]);}
    
    friend class MapUtils::DictionaryIterator<K,V>;
    public:

    Dictionary(){
        slots=1;
        count=0;
        keys=(K*)calloc(slots,sizeof(K));
        values=(V*)calloc(slots,sizeof(V));
    }
    Dictionary(int size){
        slots=size;
        count=0;
        keys=(K*)calloc(slots,sizeof(K));
        values= (V*)calloc(slots,sizeof(V));
    }
    ~Dictionary(){
        for(int i=0;i< slots;i++){ 
            if(MapUtils::IsNull(keys[i]))continue;
            UnsetEntry(i); 
        }
        free(keys);keys=nullptr;
        free(values);values=nullptr;
    }
    V Get(K key){ 
        for(int i=0;i< slots;i++){
            if(MapUtils::IsNull(keys[i]))continue;
            if(MapUtils::Compare(key,keys[i])){return values[i];}
        }
        return MapUtils::NullValue(values[0]);
    }
    bool Has(K key){return !(MapUtils::IsNull(Get(key)));}
    bool Add(K key,V value){
        if(Has(key))return false;
        if(MapUtils::IsNull(key)){logger::exception("Dictionary.Add: Invalid Key");}
        int slot_to_add=0;
        while(slot_to_add < slots && !MapUtils::IsNull(keys[slot_to_add])){slot_to_add++;}
        if(slot_to_add == slots){Resize(slots*2);}
        SetKey(slot_to_add,key);
        SetValue(slot_to_add,value);
        count++;
        return slot_to_add;
    }
    void Remove(K key){
        for(int i=0;i< slots;i++){
            if(MapUtils::IsNull(keys[i]))continue;
            if(MapUtils::Compare(key,keys[i])){
                UnsetEntry(i);
                return;
            }
        }
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
            UnsetEntry(i);
        }
        slots=0;
        count=0;
    }

    K ReverseLookup(V value){
        for(int i=0;i< slots;i++){
            if(MapUtils::IsNull(keys[i]))continue;
            if(MapUtils::Compare(value,values[i])){
                return keys[i];
            }
        }
    }

    //range-for loop iterator methods
    MapUtils::DictionaryIterator<K,V> begin(){ return {this,NextIndex(-1)};}
    MapUtils::DictionaryIterator<K,V> end(){ return {this,slots};}

    private:
    int NextIndex(int start_index){
        int next=start_index;
        while( ++next < slots && MapUtils::IsNull(keys[next]));
        return next;
    }
};

template <typename K,typename V>
Tuple<K,V> MapUtils::DictionaryIterator<K,V>::operator*(){
    if(index < 0||index >= parent->slots){logger::exception("ListIterator::Operator* -> Index %d is out of range.",index);}
    return {parent->keys[index], parent->values[index]};
}
template <typename K,typename V>
MapUtils::DictionaryIterator<K,V> MapUtils::DictionaryIterator<K,V>::operator++(){
    this->index=parent->NextIndex(index);
    return *this;
}
template <typename K,typename V>
bool MapUtils::DictionaryIterator<K,V>::operator==(DictionaryIterator<K,V>& l2){
    return index ==l2.index;
}
template <typename K,typename V>
bool MapUtils::DictionaryIterator<K,V>::operator!=(DictionaryIterator<K,V>& l2){
    return !(index ==l2.index);
}


#endif