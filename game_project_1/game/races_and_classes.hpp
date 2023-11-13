#ifndef RACES_AND_CLASSES_H
#define RACES_AND_CLASSES_H

#include <game_project_1/types/str.hpp>
#include <game_project_1/types/map.hpp>
#include <game_project_1/types/arrays.hpp>
#include <game_project_1/game/stats.hpp>
#include <game_project_1/io/json.hpp>

struct EntityRaceStyle{
    wchar* name;
    char* model_target;
    int    style_count;

    EntityRaceStyle();
    ~EntityRaceStyle();
    void FromJson(JSONObject* json);
};

struct EntityRaceColor{
    int target_count;
    char** model_targets;//todo- multiple groups at once

    EntityRaceColor();
    ~EntityRaceColor();
    void FromJson(JSONObject* json);
};

struct EntityRaceModel{
    char* model_name;
    Array<EntityRaceStyle> styles;
    Array<EntityRaceColor> colorable;
    
    EntityRaceModel();
    ~EntityRaceModel();
    void FromJson(JSONObject* json);
};

struct EntityRace{
    wchar* name;
    Array<EntityRaceModel> models;
    float hitsphere_height;
    float hitsphere_radius;
    StatLayer stat_base;

    EntityRace();
    ~EntityRace();
    void FromJson(JSONObject* json);
};

struct EntityClass{
    wchar* name;
    StatLayer stat_bonus;
    
    EntityClass();
    ~EntityClass();
    void FromJson(JSONObject* json);
};

extern Map<int,EntityClass*> Classes;
extern Map<int,EntityRace*> Races;

#endif