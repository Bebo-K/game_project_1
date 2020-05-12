#ifndef LEVEL_H
#define LEVEL_H


#include "../phys/level_collision.h"
#include "../io/json.h"
#include "../structs/data_types.h"
#include "../structs/3d_types.h"
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
	Skybox*		    skybox;
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

	void Draw(Camera* cam,mat4* view, mat4* projection);

    void LoadDefault();
    void LoadFromJSON(JSONObject* level_data);
    void Unload();
};


#endif