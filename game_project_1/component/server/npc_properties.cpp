#include <game_project_1/component/server/npc_properties.hpp>




NPCProperties::NPCProperties(){Clear();}

NPCProperties::~NPCProperties(){Clear();}

Component* NPCProperties::Clone(){
    NPCProperties* prop = new NPCProperties();
    prop->controller_type=controller_type;
    return prop;
}

void NPCProperties::Clear(){controller_type=0;}

void NPCProperties::Read(Deserializer& dat){controller_type = dat.GetInt();}

void NPCProperties::Write(Serializer& dat){dat.PutInt(controller_type);}