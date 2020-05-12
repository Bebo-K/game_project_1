#ifndef GLTF_H
#define GLTF_H

#include "file.h"

#define GLB_MAGIC_NUMBER 0x46546C67
#define JSON_CHUNK 0x4E4F534A
#define BINARY_CHUNK 0x004E4942

#define CTYPE_BYTE 5120
#define CTYPE_UBYTE 5121
#define CTYPE_SHORT 5122
#define CTYPE_USHORT 5123
#define CTYPE_UINT 5125
#define CTYPE_FLOAT 5126

#include "json.h"
#include "../structs/data_types.h"
#include "../gfx/model.h"

class GLTFScene{
	private:
	JSONObject* gltf_data;
	PointerArray binary_buffers;
	const char*	filename;

	void LoadAsGLTF(File gltf_file);
	void LoadAsGLB(File glb_file);

	char* FindMeshGroupName(int group_id);
	JSONObject* GetAccessor(int id);
	byte* GetBufferViewData(int buffer_view_id,int* len);
	VBO  BuildAccessorBuffer(int id,GLuint bufferType);
	float* BuildAccessorFloatArray(int id,int* count);
	int* BuildAccessorIntArray(int id,int* count);
	void LoadAnimation(int animation_id,Animation* dest);
	
	Texture		GetTexture(int texture_id);
	Material  	GetMaterial(int material_id);
	Skeleton* 	GetSkeleton(int skeleton_id);
	void		GetMeshGroup(MeshGroup* group, int group_id);
	void 		GetModel(ModelData* model);

	public:

	GLTFScene(File model_file);
	
	ModelData* Load();
	void LoadIn(ModelData* dest);

	~GLTFScene();
};


#endif
