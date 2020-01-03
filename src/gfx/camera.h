#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"
#include "gload.h"
#include "../io/file.h"

class Shader{
    public:

    GLuint program;

	static const int MAX_BONES = 32;

	//attribute IDs
	static const GLuint ATTRIB_VERTEX = 0;
	static const GLuint ATTRIB_TEXCOORD = 1;
	static const GLuint ATTRIB_NORMAL = 2;
	static const GLuint ATTRIB_POSE_INDEX=3;
	
	//uniform IDs
	GLuint MODELVIEW_MATRIX;
	GLuint PROJECTION_MATRIX;
	GLuint POSE_MATRIX[MAX_BONES];
    GLuint TEXTURE_0;
	GLuint IMAGE_SIZE;
	GLuint TEXTURE_LOCATION;
	GLuint AMBIENT;
	GLuint DIFFUSE;
	GLuint SPECULAR;
	GLuint EMISSIVE;
	//public int SHININESS;

	Shader();
    Shader(const char* vertexFile,const char* fragmentFile);
	void Use();
};

class Camera{
    public:
    
    float x,y,z;
    vec3  rotation;
    
    bool ortho;
    float width,height;
    float fov;
    float near_clip;
    float far_clip;
    Shader* shader;

    Camera();
    vec3 ToWorldSpace(vec3 v);
    void ToCameraSpace(mat4* m);


};


#endif