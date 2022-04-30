#ifndef LEVEL_H
#define LEVEL_H

#include <game_project_1/types/arrays.hpp>
#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/io/json.hpp>

struct LevelEntrance{
    vec3 pos;
    vec3 size;
    float y_turn;
    //float y_turn deviation;
    int style;
    vec3 target_pos;//for entrance cutscenes

    Location GenerateLocation();
};

struct LevelExit{
    char* trigger_collider;
    int mesh_index;
    int new_area_id;
    int entrance_id;
    int style;
    vec3 target_pos;//for exit cutscenes where we exit by running somewhere
    LevelExit();
    ~LevelExit();
};

struct LevelTrigger{
    char* trigger_collider;
    int mesh_index;
    int effect_id;
    LevelTrigger();
    ~LevelTrigger();
};

class LevelLoader{
    JSONObject* json; 
    public:

    LevelLoader(int area_id);
    ~LevelLoader();

    void                 LoadSkybox(Skybox* skybox);
    Array<ModelData>     LoadModels();
    Array<MeshCollider> LoadCollisionMeshes();
    Array<HeightMapCollider> LoadHeightmaps();
    Array<LevelEntrance> LoadEntrances();
    Array<LevelExit> LoadExits();
    Array<LevelTrigger> LoadTriggers();



};



#endif