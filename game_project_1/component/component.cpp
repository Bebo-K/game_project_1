#include <game_project_1/component/component.hpp>


Component::~Component(){}

byte* Component::Serialize(int* size){
    if(size != nullptr){(*size)=0;}
    return null;
}

void  Component::Deserialize(byte* data,int size){}