#ifndef MODEL_H
#define MODEL_H

#include "model_registry.h"
#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "drawable.h"
#include "skeleton.h"

struct Mesh{
    Material   mat;
    VBO      vertex;
    VBO      index;
    VBO      texcoord_0;
    VBO      normal;
    VBO      bone_0_index;
    VBO      bone_0_weight;
	int		 vertex_count;// must be a multiple of 3 ('cause triangles)
};

struct MeshGroup{
    char* name;
    int mesh_count;
    Mesh* meshes;
    AABB  bounds;
};

class ModelData{
    public:
	static const int MAX_BONES = 32;
    
    char*       name;//Pointer shared across clones.
    int         mesh_group_count;
	MeshGroup*  mesh_groups;//Pointer shared across clones.
    AABB        bounds;
    Skeleton*   skeleton;
    int         users;

    ModelData();
    ~ModelData();
    void DrawMesh(Camera* cam,int group_index,int mesh_index);
};

class Model: public Drawable{
    public:
    ModelData* data;
    Pose* pose;

    Model(ModelData* dat);
    ~Model();
    void Draw(Camera* cam,mat4* view, mat4* projection);

    
    static void Register(ModelID type,const char* filename);
    static Model* Instantiate(ModelID type);
    static Model* ErrorModel();
    //void Destroy();
};

#endif