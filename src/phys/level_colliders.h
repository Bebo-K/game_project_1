#ifndef LEVEL_COLLIDERS_H
#define LEVEL_COLLIDERS_H

#include "../struct/3d_types.h"
#include "../struct/math_types.h"
#include "../io/image.h"
#include "../gfx/model.h"
#include "../game/entity.h"
#include "collider.h"
#include "collision_types.h"


class CollisionMesh{ 
    public:
    char*               name;
    CollisionSurface    surface;
    AABB                bounds;
	int		            vertex_count;// must be a multiple of 3 ('cause triangles)
    float*              vertices;

    CollisionMesh();
    CollisionMesh(MeshGroup* mesh,CollisionSurface* surface);
    ~CollisionMesh();

    void SetVertices(MeshGroup* mesh);
    void SetSurface(CollisionSurface* surface);
    CollisionList* CheckCollisions(Entity* e,vec3 step_pos);
    void CheckOOB(Entity* e);
};

class HeightMap{
    public:
    CollisionSurface    surface;
    AABB                bounds;

    float               width,depth,height;
    int                 sample_width,sample_height;
    float*              samples;//0 to 1

    HeightMap();
    HeightMap(float width, float height, float depth, Image* img,CollisionSurface surface);
    CollisionList* CheckCollisions(Entity* e,vec3 step_pos);
};

HeightMap MakeDeathPlane(float z_pos);



#endif