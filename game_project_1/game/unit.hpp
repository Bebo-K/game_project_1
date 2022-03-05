#ifndef UNIT_H
#define UNIT_H

struct color{
    byte r,g,b,a;
};

struct UnitAppearance{
    int style_options;
    int* styles;

    //float range_options
    //float* range
    
    int color_count;
    color* colors;

    UnitAppearance();
    void operator=(UnitAppearance& b);
};

class GameUnit{
    wchar* name;
    int race;
    UnitAppearance appearance;
    // UnitInventory inventory
    // UnitStats stats

    GameUnit();
};



#endif