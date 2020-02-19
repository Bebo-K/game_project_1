#ifndef MODEL_H
#define MODEL_H

#include "3dstructs.h"
#include "primitive.h"
#include "skeleton.h"

struct Mesh{
    Material   mat;
    GLuint      index_buffer;
    GLuint      vertex_buffer;
    GLuint      texcoord_0_buffer;
    GLuint      normal_buffer;
    GLuint      bone_0_index_buffer;
    GLuint      bone_0_weight_buffer;
	int		    element_count;// must be a multiple of 3 ('cause triangles)
};


struct MeshGroup{
    char* name;
    int mesh_count;
    Mesh* meshes;
    AABB  bounds;
};

class Model: public Primitive{
    public:
	static const int MAX_BONES = 32;
    
    char*       name;//Pointer shared across clones.
    int         mesh_group_count;
	MeshGroup*  mesh_groups;//Pointer shared across clones.
    AABB        bounds;
    Skeleton*   skeleton;

    Model();
    Model* Clone();//Same shared data among all clones.
    void DestroySharedData();
    ~Model();

    void Draw(Camera* cam,mat4* view, mat4* projection);
    void DrawMesh(Camera* cam,Mesh* mesh);
};

namespace ModelManager{
    void Init();

    void Add(const char* name, Model* model);
    Model* Get(const char* name);
    void Remove(const char* name);
    Model* ErrorModel();
};

#endif