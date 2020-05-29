#ifndef MODEL_H
#define MODEL_H


#include "../structs/3d_types.h"
#include "../structs/math_types.h"
#include "drawable.h"
#include "skeleton.h"

enum ModelID{
    NONE=0,
    PLAYER_PLACEHOLDER=1
};

struct Mesh{
    Material   mat;
    VBO      vertex;
    VBO      index;
    VBO      texcoord_0;
    VBO      normal;
    VBO      bone_0_index;
    VBO      bone_0_weight;
	int		 vertex_count;// must be a multiple of 3 ('cause triangles)

    Mesh();
    ~Mesh();
};

struct MeshGroup{
    char* name;
    int mesh_count;
    Mesh* meshes;
    AABB  bounds;
    MeshGroup();
    ~MeshGroup();
};

class ModelData{
    public:
	static const int MAX_BONES = 32;
    
    int         mesh_group_count;
	MeshGroup*  mesh_groups;
    AABB        bounds;
    Skeleton*   skeleton;

    ModelData();
    ~ModelData();

    void DrawMesh(Camera* cam,int group_index,int mesh_index);
};

class Model: public Drawable{
    public:
    ModelID     type_id;
    ModelData*  data;
    Pose*       pose;
    //CONSIDER- MeshGroup visibility mask
    Model(ModelID type);
    Model(ModelData* dat);
    ~Model();
    void Draw(Camera* cam,mat4* view, mat4* projection);
    void StartAnimation(char* anim_name);

};


struct ModelCacheEntry{
    ModelID     id;
    const char* filename;
    ModelData*  data;
    int         users;
};

namespace ModelManager{
    void Init();
    void Register(ModelID id,const char* filename);
    ModelData* Use(ModelID type);
    void Return(ModelID type);
    void Clean();
    void Unregister(ModelID id);
    ModelData* ErrorModel();
};

#endif