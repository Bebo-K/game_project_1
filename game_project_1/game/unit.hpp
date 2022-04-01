#ifndef UNIT_H
#define UNIT_H

#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/str.hpp>

struct color{
    byte r,g,b,a;
    void from_bytes(byte R, byte G, byte B, byte A);
    void from_int(int color_code);
    int to_int();
};

struct UnitAppearance{
    int style_options;
    int* styles;

    //float range_options
    //float* range
    
    int color_count;
    color* colors;

    UnitAppearance();
    ~UnitAppearance();
    void operator=(UnitAppearance& b);
};

class GameUnit{
    public:
    int entity_id;
    wchar* name;
    int race_id;
    int class_id;
    UnitAppearance appearance;
    // UnitInventory inventory
    // UnitStats stats

    GameUnit();
    ~GameUnit();
};



#endif