#ifndef GLTF_H
#define GLTF_H

#include "file.h"


#define JSON_CHUNK 0x4E4F534A
#define BINARY_CHUNK 0x004E4942

#define CTYPE_BYTE 5120
#define CTYPE_UBYTE 5121
#define CTYPE_SHORT 5122
#define CTYPE_USHORT 5123
#define CTYPE_UINT 5125
#define CTYPE_FLOAT 5126

#include "json.h"

namespace GLTFScene{
	void Load(File file);
	JSONObject* ParseJsonChunk(byte* data,int length);
	void GetMeshes(JSONObject* asset,byte* buffer);
};


#endif
