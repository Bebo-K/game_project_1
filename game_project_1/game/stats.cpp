#include <game_project_1/game/stats.hpp>
#include <game_project_1/game/dice.hpp>


Stats::Stats(){

}
Stats::Stats(int mhp,int mmp,int str,int intel,int agi){
    hp = max_hp = mhp;
    mp = max_mp = mmp;
    strength = str;
    intelligence = intel;
    agility = agi;
}
Stats::~Stats(){}
void Stats::AddRandomizedBonus(int max_bonus){
    int bonus = Dice::RollRange(0,max_bonus);
    max_hp += bonus; hp += bonus;bonus = Dice::RollRange(0,max_bonus);
    max_mp += bonus; mp += bonus;bonus = Dice::RollRange(0,max_bonus);
    strength += bonus; bonus = Dice::RollRange(0,max_bonus);
    intelligence += bonus;bonus = Dice::RollRange(0,max_bonus);
    agility += bonus;
}
void Stats::Copy(Stats* s2){
    hp=s2->hp;max_hp=s2->max_hp;
    mp=s2->mp;max_mp=s2->max_mp;
    strength=s2->strength;
    intelligence=s2->intelligence;
    agility=s2->agility;
}
void Stats::Add(Stats* s2){
    hp+=s2->hp;max_hp+=s2->max_hp;
    mp+=s2->mp;max_mp+=s2->max_mp;
    strength+=s2->strength;
    intelligence+=s2->intelligence;
    agility+=s2->agility;
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