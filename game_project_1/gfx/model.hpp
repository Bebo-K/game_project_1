#ifndef MODEL_H
#define MODEL_H


#include <game_project_1/types/primitives.hpp>
#include <game_project_1/types/math_types.hpp>
#include <game_project_1/gfx/drawable.hpp>
#include <game_project_1/gfx/skeleton.hpp>


struct MeshGroupRenderOptions{
    bool hide;
    color_f color;
    MeshGroupRenderOptions();
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
    ~Mesh();
    void Init();
    void DebugPrint();
    void Draw(Shader* s,MeshGroupRenderOptions* mgro);//Assumes uniform matricies have already positioned the mesh
};

struct MeshGroup{
    char* name;
    Array<Mesh> meshes;
    AABB  bounds;
    MeshGroup();
    ~MeshGroup();
    void DebugPrint();
};

class ModelData{
    public:
	static const int MAX_BONES = 32;
    
    Array<MeshGroup> mesh_groups;
    AABB        bounds;
    Skeleton*   skeleton;

    ModelData();
    ~ModelData();
    void DebugPrint();
};


typedef int ModelID;//0 is reserved for no/error model
class Model: public Drawable{
    public:
    ModelID     type_id;
    ModelData*  data;
    Pose*       pose;
    Array<MeshGroupRenderOptions> mgro;

    Model(ModelID type);
    Model(ModelData* dat);
    ~Model();
    virtual void Draw(Camera* cam);
    void StartAnimation(char* anim_name);
    void QueueAnimation(char* anim_name);
    //void StartAnimationFromOffset(char* anim_name,AnimationOptions options, float elapsed);
    void StopAnimations();

};


struct ModelCacheEntry{
    ModelID     id;
    char* uri;
    ModelData*  data;
    int         users;
};

namespace ModelManager{
    void Init();
    void Free();
    void Register(ModelID id,char* uri);
    ModelID GetByName(char* name);
    ModelData* Use(ModelID type);
    void Return(ModelID type);
    void Clean();
    void Unregister(ModelID id);
    ModelData* ErrorModel();
};

#endif