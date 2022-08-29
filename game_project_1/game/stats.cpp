#include <game_project_1/game/stats.hpp>
#include <game_project_1/game/dice.hpp>


BaseStats::BaseStats(){}
BaseStats::BaseStats(int mhp,int mmp,int str,int intel,int agi){
    max_hp = mhp;
    max_mp = mmp;
    strength = str;
    intelligence = intel;
    agility = agi;
}
BaseStats::~BaseStats(){}
void BaseStats::AddRandomizedBonus(int max_bonus){
    int bonus = Dice::RollRange(0,max_bonus);
    max_hp += bonus; bonus = Dice::RollRange(0,max_bonus);
    max_mp += bonus; bonus = Dice::RollRange(0,max_bonus);
    strength += bonus; bonus = Dice::RollRange(0,max_bonus);
    intelligence += bonus;bonus = Dice::RollRange(0,max_bonus);
    agility += bonus;
}
void BaseStats::Copy(BaseStats* s2){
    max_hp=s2->max_hp;
    max_mp=s2->max_mp;
    strength=s2->strength;
    intelligence=s2->intelligence;
    agility=s2->agility;
}
void BaseStats::Add(BaseStats* s2){
    max_hp+=s2->max_hp;
    max_mp+=s2->max_mp;
    strength+=s2->strength;
    intelligence+=s2->intelligence;
    agility+=s2->agility;
}
int BaseStats::SerializedLength(){return sizeof(BaseStats);}

ActiveStats::ActiveStats(BaseStats* base){
    hp = base->max_hp;
    mp = base->max_mp;

    strength = base->strength;
    agility = base->agility;
    intelligence = base->intelligence;
}

CharacterAppearance::CharacterAppearance(){
    style1=0;style2=0;style3=0;
    color1.from_bytes(0,0,0,255);
}
CharacterAppearance::~CharacterAppearance(){}
void CharacterAppearance::Copy(CharacterAppearance* a2){
    style1=a2->style1;style2=a2->style2;style3=a2->style3;
    color1=a2->color1;
}