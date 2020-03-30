#ifndef LEVEL_H
#define LEVEL_H

#include "../io/json.h"
#include "../structs/data_types.h"
#include "../structs/3d_types.h"
#include "heightmap.h"


class Level{
    public: 
	//Skybox*				skybox;
    Heightmap* terrain; // 1 max
    int landmark_count;
    //CollisionMesh*        landmarks;
    //TaggedArea*           tags;?

    void LoadFromJSON(JSONObject level_data);

};


#endif