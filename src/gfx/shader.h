#ifndef SHADER_H
#define SHADER_H

#include "gload.h"
#include "../io/file.h"

class Shader{
    public:

    static const int MAX_INFO_LOG_LENGTH = 1024;
	static const int MAX_BONES = 64;

	//attribute IDs
	static const GLuint ATTRIB_VERTEX = 0;
	static const GLuint ATTRIB_TEXCOORD = 1;
	static const GLuint ATTRIB_NORMAL = 2;
	static const GLuint ATTRIB_BONE_INDEX=3;
	static const GLuint ATTRIB_BONE_WEIGHT=4;
	static const GLuint ATTRIB_COLOR=5;
	
    //Shader program ID
	GLuint vertex,fragment;
    GLuint program;
	//uniform IDs
	GLuint MODELVIEW_MATRIX;
	GLuint PROJECTION_MATRIX;
	GLuint NORMAL_MATRIX;
	GLuint POSE_MATRICES;
    GLuint TEXTURE_0;
	GLuint IMAGE_POS;
	GLuint IMAGE_SIZE;
	GLuint WINDOW_SIZE;
	GLuint TEXTURE_LOCATION;
	GLuint COLOR;
	GLuint AMBIENT;
	GLuint DIFFUSE;
	GLuint SPECULAR;
	GLuint EMISSIVE;
	//public int SHININESS;

    Shader(char* vertex_uri,char* fragment_uri);
	~Shader();
	void Use();
};

namespace ShaderManager{
	void Init();
	void Free();
    void AddShader(char* name,char* vertex_uri,char* fragment_uri);
	Shader* UseShader(char* name);
    void RemoveShader(char* name);
    //Shader* GetShader(char* name);
    //Shader* DefaultShader();
};


#endif