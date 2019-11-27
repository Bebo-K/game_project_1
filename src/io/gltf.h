#ifndef GLTF_H
#define GLTF_H

#include "file.h"


#define JSON_CHUNK 0x4E4F534A
#define BINARY_CHUNK 0x004E4942

typedef unsigned int uint32;
typedef unsigned char byte;

using namespace std;
#include <stdlib.h>

class GLTFScene{
	void load(File file);
	void parse_json_chunk(byte* data);
	void parse_binary_chunk(byte* data);
};


#endif
