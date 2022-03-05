#ifndef LEVEL_H
#define LEVEL_H

#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/gfx/renderer.hpp>
#include <game_project_1/gfx/skybox.hpp>


class Entrance{
    public:
    char*       name;
    Transform   position;
    int         style;
    vec3        target_pos;
};

class Exit{
    public:
    char*       name;
    char*       destination;
    char*       to_entrance;
    CollisionMesh* trigger;
    int         style;
    vec3        target_pos;
};

class Level : public Drawable{
    public: 
    //Heightmap     terrain;
    int             model_count;
    ModelData*      models;

    int             geometry_count; 
    CollisionMesh*  geometry;
    int             entrance_count;
    Entrance*       entrances;
    int             exit_count;
    Exit*           exits;

    Level();
    Entrance GetEntranceByName(char* name);
    Exit GetExitByName(char* name);

    void Draw(Camera* cam);
    void Unload();
};


#endif