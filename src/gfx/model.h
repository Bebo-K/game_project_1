#ifndef MODEL_H
#define MODEL_H


#include "../struct/3d_types.h"
#include "../struct/math_types.h"
#include "drawable.h"
#include "skeleton.h"

enum ModelID{
    NONE=0,
    PLAYER=1
};

struct Mesh{
	int		 vertex_count;
    int      tri_count;
    Material   mat;
    GLuint   vertex_array_id;
    VBO      vertex;
    VBO      index;
    VBO      vertex_colors;
    VBO      texcoord_0;
    VBO      normal;
    VBO      bone_0_index;
    VBO      bone_0_weight;

    Mesh();
    void Init();
    void DebugPrint();
    ~Mesh();
};

struct MeshGroup{
    char* name;
    int mesh_count;
    Mesh* meshes;
    AABB  bounds;
    MeshGroup();
    ~MeshGroup();
    void DebugPrint();
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

    void DrawMesh(Shader* shader,int group_index,int mesh_index);
    void DebugPrint();
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
    void Draw(Camera* cam);
    void StartAnimation(char* anim_name);
    void StartAnimation(char* anim_name,AnimationOptions options);

};


struct ModelCacheEntry{
    ModelID     id;
    const char* filename;
    ModelData*  data;
    int         users;
};

namespace ModelManager{
    void Init();
    void Free();
    void Register(ModelID id,const char* filename);
    ModelData* Use(ModelID type);
    void Return(ModelID type);
    void Clean();
    void Unregister(ModelID id);
    ModelData* ErrorModel();
};

#endif