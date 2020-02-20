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

#include "../data_structs.h"
#include "json.h"
#include "../gfx/model.h"

class GLTFScene{
	private:
	void LoadAsGLTF(File gltf_file);
	void LoadAsGLB(File glb_file);

	char* FindMeshGroupName(int group_id);
	JSONObject* GetAccessor(int id);
	byte* GetBufferViewData(int buffer_view_id,int* len);
	GLuint BuildAccessorBuffer(int id,GLuint bufferType);
	int* BuildIndexBuffer(int id);

	public:
	char* base_name;
	JSONObject* gltf_data;
	PointerArray binary_buffers;


	GLTFScene(File model_file);
	Material   GetMaterial(int material_id);
	MeshGroup* GetMeshGroup(int group_id);
	Skeleton*  GetSkeleton(int skeleton_id);
	
	Model* LoadAsModel(char* model_name);

	~GLTFScene();
};


#endif
