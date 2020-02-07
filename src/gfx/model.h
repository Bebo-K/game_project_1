#ifndef MODEL_H
#define MODEL_H

#include "3dstructs.h"
#include "primitive.h"

struct Mesh{
    GLuint      index_buffer;
    GLuint      vertex_buffer;
    GLuint      texcoord_0_buffer;
    GLuint      normal_buffer;
    GLuint      bone_0_index_buffer;
    GLuint      bone_0_weight_buffer;
    Material*   mat;
	int		    element_count;// must be a multiple of 3 ('cause triangles)

    void Clear();
    void Build(char* meshname,Material* m,int count,float* verts,float* texcoords, float* normals,int* bones);
    void Destroy();
};
	
class Model: public Primitive{
    public:
	static const int MAX_BONES = 32;
    
    char*   name;
    int     mesh_count;
	Mesh*   meshes;
    //char* mesh_visible;
	//Bone*   skeleton;
	//Pose    pose;
	mat4*  matrix_pallette;//this needs a more descriptive name
    AABB        bounds;
	
	//Map<String,Animation> Animations=null;

    Model();
	void SetMeshCount(int num_meshes);
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