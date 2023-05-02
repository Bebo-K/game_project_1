#ifndef IDENTITY_H
#define IDENTITY_H


#include <game_project_1/component/component.hpp>
#include <game_project_1/types/str.hpp>



typedef int EntityClass;

//Unique identifying meta info for an entity
class Identity: Component{
	public:

    EntityClass         type;
    wchar*              name;

    Identity();
    ~Identity();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);

}




#endif