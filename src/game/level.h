#ifndef LEVEL_H
#define LEVEL_H

#include "../io/json.h"
#include "../structs/data_types.h"
#include "../structs/3d_types.h"
#include "heightmap.h"
#include "../gfx/renderer.h"
#include "../gfx/skybox.h"
#include "../gfx/solid_geometry.h"


class Level{
    public: 
	Skybox*		    skybox;
    //Heightmap      terrain;
    SolidGeometry*   geometry;
    //TaggedArea*   tags;?

    Level();
    
    void AddToRenderer(Renderer* r);
    void RemoveFromRenderer(Renderer* r);

    void LoadDefault();
    void LoadFromJSON(JSONObject level_data);
    void Unload();
};


#endif