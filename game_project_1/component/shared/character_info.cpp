#include <game_project_1/component/shared/character_info.hpp>
#include <game_project_1/game/dice.hpp>

CharacterInfo::CharacterInfo(){Clear();}

CharacterInfo::~CharacterInfo(){Clear();}

int CharacterInfo::SerializedLength(){return sizeof(int)*2 + sizeof(CharacterAppearance);}

void CharacterInfo::Read(Deserializer& dat){
    race_id = dat.GetInt();
    class_id = dat.GetInt();
    appearance.body_type = dat.GetInt();
    appearance.style1 = dat.GetInt();
    appearance.style2 = dat.GetInt();
    appearance.style3 = dat.GetInt();
    appearance.color1=color(dat.GetInt());
}

void CharacterInfo::Write(Serializer& dat){
    dat.PutInt(race_id);
    dat.PutInt(class_id);
    dat.PutInt(appearance.body_type);
    dat.PutInt(appearance.style1);
    dat.PutInt(appearance.style2);
    dat.PutInt(appearance.style3);
    dat.PutInt(appearance.color1.as_code());
}

Component* CharacterInfo::Clone(ComponentParentContext context){
    CharacterInfo* copy = new CharacterInfo();
    copy->race_id = race_id;
    copy->class_id = class_id;
    copy->appearance = appearance;
    return copy;
}

void CharacterInfo::Clear(){
    race_id=0;
    class_id=0;
    appearance.Clear();
}
