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

class GLTFScene{
	private:
	void LoadAsGLTF(File gltf_file);
	void LoadAsGLB(File glb_file);
	public:
	JSONObject* gltf_data;
	PointerArray binary_buffers;

	GLTFScene(File model_file);
	byte* GetBufferData(int buffer_id,int offset);

	~GLTFScene();
};


#endif
