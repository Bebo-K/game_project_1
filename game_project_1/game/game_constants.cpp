#include <game_project_1/game/game_constants.hpp>


wchar* GameConstants::race_names[] = {L"Human",L"Golem",L"Impkin",L"Other"};
wchar* GameConstants::class_names[] =  {L"Warrior",L"Archer",L"Mage",L"Thief"};

int GameConstants::race_max_style1[]={3,3,3,3};
int GameConstants::race_max_style2[]={3,3,3,3};
int GameConstants::race_max_style3[]={3,3,3,3};

Stats GameConstants::racial_base_stats[] = {
    {12,8,10,9,11}, //human
    {11,9,12,10,8}, //golem
    {10,11,8,9,12}, //impkin
    {10,10,10,10,10} //other
};

Stats GameConstants::class_stat_bonus[] = {
    {2,0,2,0,0}, //warrior
    {0,1,0,1,2}, //archer
    {0,2,0,2,0}, //mage
    {2,0,1,0,2}  //theif
};

char* GameConstants::GetMapFileForAreaID(int area_id){
    switch(area_id){
        case 0:return nullptr;
        case STARTING_SCENE:return "default";
    }
    return nullptr;
}