#include <game_project_1/component/server/npc_properties.hpp>




NPCProperties::NPCProperties(int controller){Clear();controller_type=controller;}

NPCProperties::~NPCProperties(){Clear();}

Component* NPCProperties::Clone(){
    NPCProperties* prop = new NPCProperties(controller_type);
    prop->controller_type=controller_type;
    return prop;
}

void NPCProperties::Clear(){controller_type=0;}

void NPCProperties::Read(Deserializer& dat){controller_type = dat.GetInt();}

void NPCProperties::Write(Serializer& dat){dat.PutInt(controller_type);}