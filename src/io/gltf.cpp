#include "gltf.h"
#include "log.h"



void GLTFScene::load(File file){
	uint32 magic_number;
	uint32 version;
	uint32 length;
	
	uint32 chunk_length;
	uint32 chunk_id;
	byte* data;
	
	file.read(&magic_number,4);
	file.read(&version,4);
	file.read(&length,4);
	
	while((uint32)file.amount_read < length){
		file.read(&chunk_length,4);
		file.read(&chunk_id,4);
		data = (byte*)malloc(chunk_length);
		file.read(data,chunk_length);
		
		switch(chunk_id){
			case JSON_CHUNK:parse_json_chunk(data);break;
			case BINARY_CHUNK:parse_binary_chunk(data);break;
			default:log("GLTF unrecoginized chunk: %d, skipping", chunk_id);break;
		}
		free(data);
	}	
}

void GLTFScene::parse_json_chunk(byte* data){
	
}

void GLTFScene::parse_binary_chunk(byte* data){
	
	
}