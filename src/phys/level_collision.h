#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "../io/image.h"
#include "../gfx/model.h"
#include "../game/entity.h"
#include "collider.h"

//Level Collision data
enum LevelCollisionCase{WALL=0,FLOOR=1,CEILING=2/*,SLIDE=3*/};
enum SurfaceType{SOLID, NOCLIP, TRIGGER, WATER, DAMAGE, DEATH};
enum SurfaceMaterial{NONE,STONE};

struct CollisionSurface{
    SurfaceType     type;
    SurfaceMaterial material;
    byte*           metadata;
    int             metadata_len;
};

struct CollisionData {
    CollisionSurface*   surface;
    vec3                normal;
    vec3                velocity_cancel;//Direction to stop moving in
    vec3                shunt;//Any movement we need to make to un-clip
    LevelCollisionCase  collision_case;
    float               floor_distance;//TODO what is this?
    CollisionData*      next;
};

class CollisionMesh{ 
    public:
    char*               name;
    CollisionSurface    surface;
    AABB                bounds;
	int		            vertex_count;// must be a multiple of 3 ('cause triangles)
    float*              vertices;

    CollisionMesh(MeshGroup* mesh,CollisionSurface surface);
    CollisionData* HandleCollision(Entity e,float delta);
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
    CollisionData* HandleCollision(Entity e,float delta);
};


HeightMap MakeDeathPlane(float z_pos);



#endif