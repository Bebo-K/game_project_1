#ifndef LEVEL_H
#define LEVEL_H


#include "../phys/level_collision.h"
#include "../io/json.h"
#include "../structs/data_types.h"
#include "../structs/3d_types.h"
#include "../gfx/renderer.h"
#include "../gfx/skybox.h"


class Level{
    public: 
	Skybox*		    skybox;
    //Heightmap     terrain;
    int             model_count;
    Model*          models;

    int             geomtery_count; 
    CollisionMesh*  geometry;
    //TaggedArea*   tags;?

    Level();
    
    void AddToRenderer(Renderer* r);
    void RemoveFromRenderer(Renderer* r);

    void LoadDefault();
    void LoadFromJSON(JSONObject* level_data);
    void Unload();
};


#endif