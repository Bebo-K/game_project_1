#ifndef LEVEL_H
#define LEVEL_H

#include "../phys/level_colliders.h"
#include "../struct/data_types.h"
#include "../struct/3d_types.h"
#include "../gfx/renderer.h"
#include "../gfx/skybox.h"


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