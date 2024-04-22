#ifndef STATS_H
#define STATS_H

#include <game_project_1/types/primitives.hpp>
#include <game_project_1/io/serializer.hpp>
#include <game_project_1/io/json.hpp>

//RPG stat block
class StatLayer{
    public:
    int max_hp;
    int max_mp;

    int strength;
    int intelligence;
    int agility;

    StatLayer();
    StatLayer(int max_hp,int max_mp,int str,int intel,int agi);
    ~StatLayer();

    int SerializedLength();
    void FromJson(JSONObject* json);
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    void Clear();

    void Add(StatLayer* s2);
    void Copy(StatLayer* s2);
    void AddRandomizedBonus(int max_bonus);
};

//Fully calculated stats
class ActiveStats{
    int hp,mp;

    int strength;
    int intelligence;
    int agility;

    int iframes;

    //StatModifierStack

    ActiveStats(StatLayer* base);
    void ResetFromBase(StatLayer* base);
    int SerializedLength();
};

//Buffs,debuffs, and the like
/*
struct Effect{
    int effect_id;

};
class EffectSet : public Pool<Effect>{
	public:
	EffectSet();
	~EffectSet();
};
*/



struct CharacterAppearance{
    int body_type;
    int style1,style2,style3;
    color color1;
    //float range1,range2,range3 //TODO: float sliders to adjust a character model

    CharacterAppearance();
    ~CharacterAppearance();

    void Copy(CharacterAppearance* a2);
    void Clear();
};

#endif