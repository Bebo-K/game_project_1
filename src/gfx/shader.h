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
	static const GLuint ATTRIB_POSE_INDEX=3;
	
    //Shader program ID
	GLuint vertex,fragment;
    GLuint program;
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

    bool Load(const char* vertexFile,const char* fragmentFile);
	void Unload();
	void Use();
};

namespace ShaderManager{

	void Init();
    void AddShader(char* name, char* vertexFile,char* fragmentFile);
    Shader* GetShader(char* name);
    void RemoveShader(char* name);
    Shader* DefaultShader();


};


#endif