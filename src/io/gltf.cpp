#include "gltf.h"
#include "log.h"

void GLTFScene::Load(File file){
	uint32 magic_number;
	uint32 version;
	uint32 length;
	
	uint32 chunk_length;
	uint32 chunk_id;
	byte* data;
	
	file.read(&magic_number,4);
	file.read(&version,4);
	file.read(&length,4);

	JSONObject* asset;
	byte* binary_data;
	
	while((uint32)file.amount_read < length){
		file.read(&chunk_length,4);
		file.read(&chunk_id,4);
		data = (byte*)malloc(chunk_length);
		file.read(data,chunk_length);
		
		switch(chunk_id){
			case JSON_CHUNK:
				asset = ParseJsonChunk(data,chunk_length);
				free(data);
				break;
			case BINARY_CHUNK:
				binary_data = data;
				break;
			default:
				logger::info("GLTF unrecoginized chunk: %d, skipping", chunk_id);
				free(data);
				break;
		}
	}	

	asset->GetObject("asset")->GetString("generator")->Print(0);
	printf("\n");
}

JSONObject* GLTFScene::ParseJsonChunk(byte* data,int length){
    JSONParser parser = JSONParser((char*)data,length);

    JSONObject* obj = parser.Parse();
    //obj->print(0);
	return obj;
}

void GLTFScene::GetMeshes(JSONObject* asset,byte* buffer){
	JSONArray* objects = asset->GetArray("nodes");

	int mesh_count = 0;

	for(int i=0;i<objects->count;i++){
		JSONObject* current_object = objects->At(i)->ObjectValue();

		if(current_object->HasInt("mesh")){
			mesh_count++;
		}

	}

}