#ifndef CLIENT_LEVEL_LOADER_H
#define CLIENT_LEVEL_LOADER_H

#include <game_project_1/gfx/camera.hpp>
#include <game_project_1/gfx/skybox.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/phys/level_colliders.hpp>

class ClientLevel{
    public:
    char*           shader_name;

    int             model_count;
    ModelData*      models;

    int             collmesh_count; 
    CollisionMesh*  collmeshes;

    Skybox skybox;

    ClientLevel();
    ~ClientLevel();
    void Draw(Camera* cam);
    void LoadFromStream(Stream* stream);
    void Unload();
};




#endif