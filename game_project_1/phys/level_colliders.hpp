#ifndef LEVEL_COLLIDERS_H
#define LEVEL_COLLIDERS_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/game/entity.hpp>

namespace LevelCollision{
    enum SurfaceType{SOLID,NOCLIP,TRIGGER,WATER,DAMAGE,DEATH};
    enum Material{NONE,STONE,DIRT,SAND};
    struct Surface{
        SurfaceType type;//Action to take on colliding
        Material  material;//Particles, sound effects, etc 
        int     exit_id;
        int     damage;
        byte*   metadata;
        int     metadata_len;

        Surface();
        void SetMaterial(char* type_name,char* material_name);
    };

    enum CollisionCase{WALL=1,FLOOR=2,CEILING=4,CANCEL_VELOCITY=8};

    struct CollisionResult{
        const static int    MAX_PER_FRAME=16;//Max # of collisions to handle per frame 
        Surface*            surface;
        vec3                normal;//Tangent to collision
        vec3                velocity_cancel;//Direction to stop moving in
        vec3                shunt;//Any movement we need to make to un-clip
        int                 collision_case;//from CollisionCase enum, set as a bitflag so multiple can be true (e.g. WALL & CANCEL_VELOCITY)
        float               floor_distance;//signed distance to floor surface
                                //used in the case of multiple floors/stairs to determine where to set the entity
        static CollisionResult None();
        bool isNone();
        void Clear();
    };
}

class MeshCollider{ 
    public:
    char*                   name;
    LevelCollision::Surface surface;
    AABB                    bounds;
	int		                tri_count;
    Triangle*               tris;

    MeshCollider();
    MeshCollider(MeshGroup* mesh,LevelCollision::Surface* surface);
    ~MeshCollider();

    void ShallowCopy(MeshCollider* m2);

    void SetVertices(MeshGroup* mesh);
    void SetSurface(LevelCollision::Surface* surface);

    void CheckCollisions(BaseEntity* e,vec3 step_pos,vec3 step_velocity,LevelCollision::CollisionResult* results);
    void CheckOOB(BaseEntity* e);
};

class HeightMapCollider{
    public:
    LevelCollision::Surface surface;
    AABB                    bounds;
    float                   width,depth,height;
    int                     sample_width,sample_height;
    float*                  samples;//0 to 1

    HeightMapCollider();
    HeightMapCollider(float width, float height, float depth, Image* img,LevelCollision::Surface* surface);
    //~HeightMap();

    void CheckCollisions(BaseEntity* e,vec3 step_pos,LevelCollision::CollisionResult* list);
    //void CheckOOB(BaseEntity* e);
};
HeightMapCollider MakeDeathPlane(float z_pos);



#endif