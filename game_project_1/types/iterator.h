#ifndef ITERATOR_H
#define ITERATOR_H

template <typename Parent,typename Result>
struct Iterator{
    Parent* parent;
    int index;
    
    Iterator<Parent,Result>(Parent* p,int i){parent = p; index = i;}
    
    inline Result operator*(){return GetResult();}
    inline Iterator operator++() {
        index = Next();
        return (*this);
    }
    inline bool operator==(Iterator<Parent,Result>& l2){return index == l2.index;}
    inline bool operator!=(Iterator<Parent,Result>& l2){return index != l2.index;}

    
    Result GetResult();
    int Next(){return index + 1;}
};


#endif