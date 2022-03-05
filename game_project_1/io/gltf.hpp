#ifndef GLTF_H
#define GLTF_H

#define GLB_MAGIC_NUMBER 0x46546C67
#define JSON_CHUNK 0x4E4F534A
#define BINARY_CHUNK 0x004E4942

#define CTYPE_BYTE 5120
#define CTYPE_UBYTE 5121
#define CTYPE_SHORT 5122
#define CTYPE_USHORT 5123
#define CTYPE_UINT 5125
#define CTYPE_FLOAT 5126

#include <game_project_1/io/json.hpp>
#include <game_project_1/io/stream.hpp>
#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/gfx/model.hpp>

class GLTFScene{
	private:
	JSONObject* gltf_data;
	List<byte> binary_buffers;
	const char*	filename;

	void LoadAsGLTF(Stream* gltf_stream);
	void LoadAsGLB(Stream* gltf_stream);

	char* FindMeshGroupName(int group_id);
	JSONObject* GetAccessor(int id);
	byte* GetBufferViewData(int buffer_view_id,int* len);
	VBO  BuildAccessorBuffer(int id,GLuint bufferType);
	float* BuildAccessorFloatArray(int id,int* count);
	int* BuildAccessorIntArray(int id,int* count);
	void LoadAnimation(int animation_id,Skeleton* target);
	
	Texture		GetTexture(int texture_id);
	Material  	GetMaterial(int material_id);
	Skeleton* 	GetSkeleton(int skeleton_id);
	void		GetMeshGroup(MeshGroup* group, int group_id);
	void 		GetModel(ModelData* model);

	public:

	GLTFScene(Stream* model_stream);
	
	ModelData* Load();
	void LoadIn(ModelData* dest);

	~GLTFScene();
};


#endif
