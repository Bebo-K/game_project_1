#ifndef STATS_H
#define STATS_H

#include <game_project_1/types/data_types.hpp>

//RPG stat block
class BaseStats{
    public:
    int max_hp;
    int max_mp;

    int strength;
    int intelligence;
    int agility;

    BaseStats();
    BaseStats(int max_hp,int max_mp,int str,int intel,int agi);
    ~BaseStats();

    int SerializedLength();
    void Add(BaseStats* s2);
    void Copy(BaseStats* s2);
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

    ActiveStats(BaseStats* base);
    void ResetFromBase(BaseStats* base);
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
    int style1,style2,style3;
    color color1;
    //float range1,range2,range3 //TODO: float sliders to adjust a character model

    CharacterAppearance();
    ~CharacterAppearance();

    void Copy(CharacterAppearance* a2);
};

#endif