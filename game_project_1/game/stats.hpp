#ifndef STATS_H
#define STATS_H

#include <game_project_1/types/data_types.hpp>

//RPG stat block
class Stats{
    public:
    int hp,max_hp;
    int mp,max_mp;

    int strength;
    int intelligence;
    int agility;

    Stats();
    Stats(int max_hp,int max_mp,int str,int intel,int agi);
    ~Stats();

    int SerializedLength();
    void Add(Stats* s2);
    void Copy(Stats* s2);
    void AddRandomizedBonus(int max_bonus);
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

//Sliders to adjust a character model
struct CharacterAppearance{
    int style1,style2,style3;
    color color1;
    //float range1,range2,range3

    CharacterAppearance();
    ~CharacterAppearance();

    void Copy(CharacterAppearance* a2);
};

#endif