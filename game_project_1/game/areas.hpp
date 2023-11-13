#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <game_project_1/types/map.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/types/map.hpp>

struct Area{
    char* name;
    char* path;
    Area();
    ~Area();
    void FromJson(JSONObject* json);
};

extern Map<int,Area*> Areas;
extern Area* ERROR_ROOM;




#endif