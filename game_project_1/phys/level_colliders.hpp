#ifndef LEVEL_COLLIDERS_H
#define LEVEL_COLLIDERS_H

#include <game_project_1/types/3d_types.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/io/image.hpp>
#include <game_project_1/gfx/model.hpp>
#include <game_project_1/phys/collider.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/component/phys_components.hpp>

class CollisionMesh{ 
    public:
    char*               name;
    CollisionSurface    surface;
    AABB                bounds;
	int		            tri_count;
    Triangle*           tris;

    CollisionMesh();
    CollisionMesh(MeshGroup* mesh,CollisionSurface* surface);
    ~CollisionMesh();

    void SetVertices(MeshGroup* mesh);
    void SetSurface(CollisionSurface* surface);

    CollisionList* CheckCollisions(PhysBody* b,vec3 step_pos);
    void CheckOOB(PhysBody* b);
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
    //~HeightMap();

    CollisionList* CheckCollisions(PhysBody* b,vec3 step_pos);
    //void CheckOOB(PhysBody b);
};

HeightMap MakeDeathPlane(float z_pos);



#endif