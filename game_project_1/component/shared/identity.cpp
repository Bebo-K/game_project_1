#include <game_project_1/component/shared/identity.hpp>


Identity::Identity(){
    name=null;
    type=0;
}
Identity::~Identity(){Clear();}

Component* Identity::Clone(){
    Identity* copy = new Identity();
    copy->name = wstr::new_copy(name);
    copy->type = type;
    return copy;
}

void Identity::Clear(){
    DEALLOCATE(name)
    type=0;
}

int Identity::SerializedLength(){
    return sizeof(EntityType) + sizeof(wchar)*(1+wstr::len(name));
}
void Identity::Read(Deserializer& dat){
    type = dat.GetInt();
    name = dat.GetWString();
}
void Identity::Write(Serializer& dat){
    dat.PutInt(type);
    dat.PutWString(name);
}