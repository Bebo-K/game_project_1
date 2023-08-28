#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <game_project_1/types/arrays.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/core/level.hpp>


class ServerScene;

class LevelLoader{
    JSONObject* json; 
    public:

    LevelLoader(int area_id);
    ~LevelLoader();

    void                 LoadSkybox(Skybox* skybox);
    List<ModelData>     LoadModels();
    List<MeshCollider>  LoadCollisionMeshes();
    List<HeightMapCollider> LoadHeightmaps();
    List<LevelEntrance> LoadEntrances();
    List<LevelExit> LoadExits();
    List<LevelTrigger> LoadTriggers();

    void LoadEntities(ServerScene* scene, bool firstLoad);

};



#endif