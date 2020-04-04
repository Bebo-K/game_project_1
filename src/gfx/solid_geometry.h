#ifndef SOLID_GEOMETRY_H
#define SOLID_GEOMETRY_H

#include "primitive.h"
#include "../structs/math_types.h"

struct Surface{
    int collision_type;
    int noise_type;
};

class SolidMesh{
    public:
    Surface  surface;
    Material mat;
    float*   vertices;
    VBO      vertex;
    VBO      index;
    VBO      texcoord_0;
    VBO      normal;
	int		 vertex_count;// must be a multiple of 3 ('cause triangles)
    AABB     bounds;

    SolidMesh();
    ~SolidMesh();
};

struct SolidGroup{
    char* name;//Unique per level
    int   mesh_count;
    SolidMesh* meshes;
    AABB        bounds;
    
    SolidGroup();
    ~SolidGroup();
};

class SolidGeometry : public Primitive{//Static models used for level obstacles.
    public:
	static const int MAX_BONES = 32;
    
    int         group_count;
	SolidGroup*  groups;//Pointer shared across clones.
    AABB        bounds;

    SolidGeometry();
    ~SolidGeometry();

    void Draw(Camera* cam,mat4* view, mat4* projection);
    void DrawMesh(Camera* cam,SolidMesh* mesh);
};


#endif