#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <game_project_1/types/arrays.hpp>
#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/core/level.hpp>

#include <game_project_1/core/entity.hpp>

class LevelLoader{
    JSONObject* json; 
    public:

    LevelLoader(int area_id);
    ~LevelLoader();

    void                 LoadSkybox(Skybox* skybox);
    Array<ModelData>     LoadModels();
    Array<MeshCollider>  LoadCollisionMeshes();
    Array<HeightMapCollider> LoadHeightmaps();
    Array<LevelEntrance> LoadEntrances();
    Array<LevelExit> LoadExits();
    Array<LevelTrigger> LoadTriggers();

    void LoadEntities(Pool<ServerEntity>* entities, bool firstLoad);

};



#endif