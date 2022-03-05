#ifndef SHADER_H
#define SHADER_H

#include <game_project_1/gfx/gload.hpp>
#include <game_project_1/io/file.hpp>

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
	static const int  MAX_ATTRIBS=6;

	static const unsigned int FEATURE_DISABLE_DEPTH_TEST=0;

	int enabled_attribs_mask;
	int feature_flags;
	
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
	void SetFeature(unsigned int feature_id,bool enabled);
	void Use();
	void OnStartUse();
	void OnFinishUse();
};

namespace ShaderManager{
	void Init();
	void Free();
	Shader* AddShader(char* name);
    Shader* AddShader(char* name,char* vertex_uri,char* fragment_uri);
	Shader* UseShader(char* name);
    void RemoveShader(char* name);
    //Shader* GetShader(char* name);
    //Shader* DefaultShader();
	bool IsCurrentShader(char* name);
	Shader* CurrentShader();

}

#endif