#ifndef LEVEL_COLLISION_H
#define LEVEL_COLLISION_H

#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "../io/image.h"
#include "../gfx/model.h"
#include "../game/entity.h"
#include "collider.h"

//Level Collision data
namespace LevelCollisionCase{
    const int WALL=0;
    const int FLOOR=1;
    const int CEILING=2;
};
namespace SurfaceType{
    const int SOLID=0;
    const int NOCLIP=1;
    const int TRIGGER=2;
    const int WATER=3;
    const int DAMAGE=4;
    const int DEATH=5;
};
namespace SurfaceMaterial{
    const int NONE=0;
    const int STONE=1;
};

struct CollisionSurface{
    int     type;
    int     material;
    byte*   metadata;
    int     metadata_len;
};

struct CollisionData {
    CollisionSurface*   surface;
    vec3                normal;
    vec3                velocity_cancel;//Direction to stop moving in
    vec3                shunt;//Any movement we need to make to un-clip
    int                 collision_case;
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

    CollisionMesh();
    CollisionMesh(MeshGroup* mesh,CollisionSurface* surface);
    ~CollisionMesh();

    void SetVertices(MeshGroup* mesh);
    void SetSurface(CollisionSurface* surface);
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