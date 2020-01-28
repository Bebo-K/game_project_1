#include "gltf.h"
#include "../log.h"
#include "../data_structs.h"

GLTFScene::GLTFScene(File model_file) :binary_buffers(1){
	uint32 magic_number;
	model_file.read(&magic_number,4);
	if(magic_number == GLB_MAGIC_NUMBER){LoadAsGLB(model_file);}
	else{LoadAsGLTF(model_file);}
}

GLTFScene::~GLTFScene(){
	delete gltf_data;
}

void GLTFScene::LoadAsGLB(File glb_file){
	uint32 magic_number;
	uint32 version;
	uint32 file_length;
	
	uint32 chunk_length;
	uint32 chunk_id;
	byte* data;
	
	glb_file.read(&magic_number,4);
	glb_file.read(&version,4);
	glb_file.read(&file_length,4);

	while((uint32)glb_file.amount_read < file_length){
		glb_file.read(&chunk_length,4);
		glb_file.read(&chunk_id,4);
		data = (byte*)malloc(chunk_length);
		glb_file.read(data,chunk_length);
		
		switch(chunk_id){
			case JSON_CHUNK:{
				JSONParser parser = JSONParser((char*)data,chunk_length);
				gltf_data = parser.Parse();
				free(data);
				break;}
			case BINARY_CHUNK:
				binary_buffers.Add(data);
				break;
			default:
				logger::info("GLB unrecoginized chunk: %d, skipping", chunk_id);
				free(data);
				break;
		}
	}	
}

void GLTFScene::LoadAsGLTF(File gltf_file){
	byte* gltf_json = (byte*)malloc(gltf_file.length);
	gltf_file.read(gltf_json,gltf_file.length);

	JSONParser parser = JSONParser((char*)gltf_json,gltf_file.length);
	gltf_data = parser.Parse();

	JSONArray* buffers = gltf_data->GetArray("buffers");
	for(int i=0;i< buffers->count;i++){
		if(buffers->At(i)->ObjectValue()->HasString("uri")){

			JSONString* bufferURI = buffers->At(i)->ObjectValue()->GetString("uri");
			binary_buffers.Add(GetRelativeFile(bufferURI->string,gltf_file.path));
		}
	}
	free(gltf_json);
}

byte* GLTFScene::GetBufferData(int buffer_id,int offset){
	return &((byte*)binary_buffers.Get(buffer_id))[offset];
}