#ifndef RPG_COMPONENTS_H
#define RPG_COMPONENTS_H

#include <game_project_1/component/component.hpp>
#include <game_project_1/game/races_and_classes.hpp>

//Public character info: race, character class, appearance
class CharacterInfo: public Component{
    public:
    int race_id;
    int class_id;
    CharacterAppearance appearance;

    CharacterInfo();
    ~CharacterInfo();
    
    Component* Clone(ComponentParentContext context);
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

#endif 