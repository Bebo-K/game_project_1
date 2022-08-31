#include <game_project_1/game/races_and_classes.hpp>

int Races::Max=4;
int Classes::Max=4;


Race Races::GetRaceByID(RaceID id){
    switch(id){
        case 0: return Races::Human;
        case 1: return Races::Golem;
        case 2: return Races::Impkin;
        case 3: return Races::Torag;
        default: return Races::Human;
    }
}

Class Classes::GetClassByID(ClassID id){
    switch(id){
        case 0: return Classes::Warrior;
        case 1: return Classes::Archer;
        case 2: return Classes::Mage;
        case 3: return Classes::Thief;
        default: return Classes::Warrior;
    }
}
