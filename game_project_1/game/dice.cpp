#include <game_project_1/game/dice.hpp>
#include <stdlib.h> 
#include <time.h>


void Dice::Init(){
    srand(time(NULL));
}

int Dice::RollRange(int lo,int hi){
    int range = lo-hi;
    if(range < 0){range = -range;}
    int range_roll = rand() % (range+1);
    return lo+range_roll;
}