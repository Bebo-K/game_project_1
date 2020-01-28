#ifndef MODEL_H
#define MODEL_H

#include "primitive.h"
#include "../io/gltf.h"

struct Mesh{
    char*       name;
    GLuint      vertex_buffer;
    GLuint      texcoord_buffer;
    GLuint      normal_buffer;
    GLuint      bone_index_buffer;
    Material*   mat;
	int		    element_count;// must be a multiple of 3 ('cause triangles)

};
	
class Model: public Primitive{
    public:
	static const int MAX_BONES = 32;
    
    int     mesh_count;
	Mesh*   meshes;
    //char* mesh_visible;
	//Bone*   skeleton;
	//Pose    pose;
	mat4*  matrix_pallette;//this needs a more descriptive name
	
	//Map<String,Animation> Animations=null;

    Model();
	Model(int num_meshes);
    ~Model();

    void Draw(Camera* cam,mat4* view, mat4* projection);
    void DrawMesh(Camera* cam,Mesh* mesh);
};

namespace ModelManager{
    void Init();

    Model* Add(const char* name, GLTFScene scene);
    Model* Get(const char* name);
    void Remove(const char* name);
    Model* ErrorModel();

    void BuildMesh(Mesh* dest,char* meshname,Material* m,int count,float* verts,float* texcoords, float* normals,int* bones);
    void DeleteMesh(Mesh* m);
};

#endif