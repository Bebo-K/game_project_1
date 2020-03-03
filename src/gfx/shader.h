#ifndef SHADER_H
#define SHADER_H

#include "gload.h"
#include "../io/file.h"

class Shader{
    public:

    static const int MAX_INFO_LOG_LENGTH = 1024;
	static const int MAX_BONES = 32;

	//attribute IDs
	static const GLuint ATTRIB_VERTEX = 0;
	static const GLuint ATTRIB_TEXCOORD = 1;
	static const GLuint ATTRIB_NORMAL = 2;
	static const GLuint ATTRIB_BONE_INDEX=3;
	//static const GLuint ATTRIB_BONE_0_WEIGHT=4;
	//static const GLuint ATTRIB_BONE_1_INDEX=5;
	//static const GLuint ATTRIB_BONE_1_WEIGHT=6;
	
    //Shader program ID
	GLuint vertex,fragment;
    GLuint program;
	//uniform IDs
	GLuint MODELVIEW_MATRIX;
	GLuint PROJECTION_MATRIX;
	GLuint NORMAL_MATRIX;
	GLuint POSE_MATRICES;
    GLuint TEXTURE_0;
	GLuint IMAGE_SIZE;
	GLuint TEXTURE_LOCATION;
	GLuint AMBIENT;
	GLuint DIFFUSE;
	GLuint SPECULAR;
	GLuint EMISSIVE;
	//public int SHININESS;

    Shader(const char* vertexFile,const char* fragmentFile);
	~Shader();
	void Use();
};

namespace ShaderManager{
	void Init();
    void AddShader(const char* name,const char* vertexFile,const char* fragmentFile);
    Shader* GetShader(const char* name);
    void RemoveShader(const char* name);
    Shader* DefaultShader();

};


#endif