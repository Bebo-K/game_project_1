#include <game_project_1/component/shared/character.hpp>
#include <game_project_1/game/dice.hpp>

Character::Character(){Clear();}

Character::~Character(){Clear();}

int Character::SerializedLength(){return sizeof(int)*2 + sizeof(CharacterAppearance);}

void Character::Read(Deserializer& dat){
    race_id = dat.GetInt();
    class_id = dat.GetInt();
    appearance.style1 = dat.GetInt();
    appearance.style2 = dat.GetInt();
    appearance.style3 = dat.GetInt();
    appearance.color1.rgba(dat.GetInt());
}

void Character::Write(Serializer& dat){
    dat.PutInt(race_id);
    dat.PutInt(class_id);
    dat.PutInt(appearance.style1);
    dat.PutInt(appearance.style2);
    dat.PutInt(appearance.style3);
    dat.PutInt(appearance.color1.as_code());
}

Component* Character::Clone(){
    Character* copy = new Character();
    copy->race_id = race_id;
    copy->class_id = class_id;
    copy->appearance = appearance;
    return copy;
}

void Character::Clear(){
    race_id=0;
    class_id=0;
    appearance.Clear();
}
