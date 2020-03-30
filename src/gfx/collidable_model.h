#ifndef COLLIDABLE_MODEL_H
#define COLLIDABLE_MODEL_H

#include "primitive.h"
#include "../structs/math_types.h"

class CollidableMesh{
    Material   mat;
    VBO      vertex;
    VBO      index;
    VBO      texcoord_0;
    VBO      normal;
    VBO      bone_0_index;
    VBO      bone_0_weight;
	int		 vertex_count;// must be a multiple of 3 ('cause triangles)
};

class CollidableModel : Primitive{//Static models used for level obstacles.
    public:
	static const int MAX_BONES = 32;
    
    char*       name;//Pointer shared across clones.
    int         mesh_count;
	CollidableMesh*  meshes;//Pointer shared across clones.
    AABB        bounds;


    CollidableModel();
    ~CollidableModel();

    void Draw(Camera* cam,mat4* view, mat4* projection);
    void DrawMesh(Camera* cam,CollidableMesh* mesh);
};


#endif