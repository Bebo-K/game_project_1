#include "gltf.h"
#include "../log.h"
#include "../data_structs.h"

GLTFScene::GLTFScene(File model_file) :binary_buffers(1){
	if(model_file.error)return;
	uint32 magic_number;
	model_file.peek(&magic_number,4);
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

JSONObject* GLTFScene::GetAccessor(int id){
	JSONArray* accessors = gltf_data->GetArray("accessors");
	return accessors->At(id)->ObjectValue();
}

int* GLTFScene::BuildIndexBuffer(int id){
	GLuint ret = -1;
	JSONObject* accessor = gltf_data->GetArray("accessors")->At(id)->ObjectValue();
	int element_size=4;
	int element_count = accessor->GetInt("count");
	return null;
}

GLuint GLTFScene::BuildAccessorBuffer(int id,GLuint bufferType){
	GLuint ret = -1;
	JSONObject* accessor = gltf_data->GetArray("accessors")->At(id)->ObjectValue();
	int element_size=4;
	int element_count = accessor->GetInt("count");

	switch(accessor->GetInt("componentType")){
		case 5120:element_size=1;break;
		case 5121:element_size=1;break;
		case 5122:element_size=2;break;
		case 5123:element_size=2;break;
		case 5126:element_size=4;break;
		case 5125:element_size=4;break;
		default:element_size=4;break;
	}
	
	char* type = accessor->GetString("type")->string;
	if(cstr::compare(type,"SCALAR")){element_size *= 1;}
	if(cstr::compare(type,"VEC2")){element_size *= 2;}
	if(cstr::compare(type,"VEC3")){element_size *= 3;}
	if(cstr::compare(type,"VEC4")){element_size *= 4;}
	if(cstr::compare(type,"MAT2")){element_size *= 4;}
	if(cstr::compare(type,"MAT3")){element_size *= 9;}
	if(cstr::compare(type,"MAT4")){element_size *= 16;}

	byte* element_data = null;
	byte* new_data = null;
	if(accessor->HasInt("bufferView")){
		int element_buffer = accessor->GetInt("bufferView");
		element_data=GetBufferViewData(element_buffer,nullptr) ;
	}
	if(accessor->HasJObject("sparse")){
		JSONObject* sparse = accessor->GetJObject("sparse");
		int sparse_count= sparse->GetInt("count");
		if(element_data == null){
			new_data=(byte*)calloc(element_size,element_count);
			element_data = new_data;
		}
		
		JSONObject* indices_obj = sparse->GetJObject("indices");
		byte* indices_data = GetBufferViewData(indices_obj->GetInt("bufferView"),nullptr);
		if(indices_obj->HasInt("byteOffset")){
			indices_data = &indices_data[indices_obj->GetInt("byteOffset")];
		}
		int* indices = (int*)indices_data;

		JSONObject* values_obj = sparse->GetJObject("indices");
		byte* values_data = GetBufferViewData(values_obj->GetInt("bufferView"),nullptr);
		if(values_obj->HasInt("byteOffset")){
			values_data = &values_data[values_obj->GetInt("byteOffset")];
		}
		
		for(int i=0;i<sparse_count;i++){
			memcpy(&element_data[element_size*indices[i]],&values_data[element_size*i],element_size);
		}
	}
	if(accessor->HasInt("byteOffset")){
		element_data = &element_data[accessor->GetInt("byteOffset")];
	}

	glGenBuffers(1,&ret);
    glBindBuffer(bufferType, ret);
    glBufferData(bufferType, element_size*element_count, element_data ,GL_STATIC_DRAW);

	logger::info("\n");
	float first,second,third;
	for(int i=0; i<element_count;i++){
		first = ((float*)element_data)[i*3];
		second = ((float*)element_data)[i*3 +1];
		third = ((float*)element_data)[i*3 +2];

		logger::info("%d,%d,%d\n",first,second,third);
	}
		logger::info("\n");
		logger::flush();

	if(new_data){free(new_data);}
	return ret;
}

byte* GLTFScene::GetBufferViewData(int buffer_view_id,int* len){
	JSONArray* bufferviews = gltf_data->GetArray("bufferViews");
	JSONObject* bufferview = bufferviews->At(buffer_view_id)->ObjectValue();
	int buffer_id= bufferview->GetInt("buffer");
	int offset= bufferview->GetInt("byteOffset");
	if(len != null){*len = bufferview->GetInt("byteLength");}
	byte* buffer = (byte*)binary_buffers.Get(buffer_id);
	return &buffer[offset];
}

Material* GLTFScene::GetMaterial(int id){
	Material* ret = new Material();
	if(!gltf_data->HasArray("materials")){return ret;}
	JSONArray* materials = gltf_data->GetArray("materials");
	if(materials->count < id || id < 0){return ret;}
	JSONObject* material = materials->At(id)->ObjectValue();
	
	if(material->HasBool("doubleSided")){
		ret->cull_backface=!(material->GetBool("doubleSided"));
	}
	if(material->HasJObject("pbrMetallicRoughness")){
		JSONObject* PBRMaterial = material->GetJObject("pbrMetallicRoughness");
		JSONArray* baseColor=PBRMaterial->GetArray("baseColorFactor");	
			ret->base_color[0] = baseColor->At(0)->FloatValue();
			ret->base_color[1] = baseColor->At(1)->FloatValue();
			ret->base_color[2] = baseColor->At(2)->FloatValue();
			ret->base_color[3] = baseColor->At(3)->FloatValue();
		ret->metallic_factor=PBRMaterial->GetFloat("metallicFactor");
		ret->roughness_factor=PBRMaterial->GetFloat("roughnessFactor");
	}
	
	//TODO: textures

	return ret;
}

int GLTFScene::GetModels(Model* models,int max_count){return GetModels(models,max_count,0);}
int GLTFScene::GetModels(Model* models,int max_count,int start_index){
	JSONArray* gltf_meshes = gltf_data->GetArray("meshes");
	int get_count = (max_count > gltf_meshes->count)?gltf_meshes->count:max_count;

	for(int mdl=start_index;mdl<(start_index+get_count);mdl++){
		JSONObject* gltf_mesh = gltf_meshes->At(mdl)->ObjectValue();
		JSONArray* primitives = gltf_mesh->GetArray("primitives");
		JSONObject* primitive = null; 
		models[mdl].name = cstr::new_copy(gltf_mesh->GetString("name")->string);
		Mesh* meshes = (Mesh*)calloc(primitives->count,sizeof(Mesh));
		
		models[mdl].meshes = meshes;
		models[mdl].mesh_count = primitives->count;	
		for(int msh=0;msh<primitives->count;msh++){
			Mesh* mesh = &meshes[msh];
			primitive = primitives->At(msh)->ObjectValue();

			JSONObject* attribs = primitive->GetJObject("attributes");
				int pos_attrib_id=attribs->GetInt("POSITION");
				JSONObject* pos_accessor = GetAccessor(pos_attrib_id);
				mesh->element_count = pos_accessor->GetInt("count");

				JSONArray* max_array = pos_accessor->GetArray("max");
				JSONArray* min_array = pos_accessor->GetArray("min");
				AABB mesh_bounds;
					mesh_bounds.hi_corner.x= max_array->At(0)->FloatValue();
					mesh_bounds.hi_corner.y= max_array->At(1)->FloatValue();
					mesh_bounds.hi_corner.z= max_array->At(2)->FloatValue();
					mesh_bounds.lo_corner.x= min_array->At(0)->FloatValue();
					mesh_bounds.lo_corner.y= min_array->At(1)->FloatValue();
					mesh_bounds.lo_corner.z= min_array->At(2)->FloatValue();
				models[mdl].bounds.Union(mesh_bounds);

				mesh->vertex_buffer=BuildAccessorBuffer(pos_attrib_id,GL_ARRAY_BUFFER);
				if(attribs->HasInt("NORMAL")){
					mesh->normal_buffer=BuildAccessorBuffer(attribs->GetInt("NORMAL"),GL_ARRAY_BUFFER);}
				if(attribs->HasInt("TEXCOORD_0")){
					mesh->texcoord_0_buffer=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_0"),GL_ARRAY_BUFFER);}
				if(attribs->HasInt("JOINTS_0")){
					mesh->bone_0_index_buffer=BuildAccessorBuffer(attribs->GetInt("JOINTS_0"),GL_ARRAY_BUFFER);}
				if(attribs->HasInt("WEIGHTS_0")){
					mesh->bone_0_weight_buffer=BuildAccessorBuffer(attribs->GetInt("WEIGHTS_0"),GL_ARRAY_BUFFER);}
				//if(attribs->HasInt("TEXCOORD_1")){
				//	mesh->texcoord_0_buffer=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_1"));}
				
			mesh->mat = GetMaterial(primitive->GetInt("material"));
			if(primitive->HasInt("indices")){
				int index_accessor_id=primitive->GetInt("indices"); 
				mesh->index_buffer = BuildAccessorBuffer(index_accessor_id,GL_ELEMENT_ARRAY_BUFFER);
				mesh->element_count = GetAccessor(index_accessor_id)->GetInt("count");
			}
		}		
	}
	return get_count;
}

Skeleton* GLTFScene::GetSkeleton(int skin_id){
	if(!gltf_data->HasArray("skins"))return null;

	JSONArray* skins = gltf_data->GetArray("skins");
	JSONObject* skin = skins->At(skin_id)->ObjectValue;
	JSONArray* nodes_array = gltf_data->GetArray("nodes");

	if(skin_id < 0 || skin_id > skins->count)return null;

	Skeleton* new_skeleton = new Skeleton();
	char* new_skeleton_name = skin->GetString("name")->string;
	JSONArray* joint_nodes = skin->GetArray("joints");
	new_skeleton->Setup(joint_nodes->count);

	int ibm_data_buffer_id = GetAccessor(skin->GetInt("inverseBindMatrices"))->GetInt("bufferView");
	int read =0;
	byte* ibm_data_buffer=GetBufferViewData(ibm_data_buffer_id,&read);
	int ibm_expected_size = sizeof(mat4)*new_skeleton->bone_count;
	if(ibm_expected_size != read){
		logger::exception("Expected inverse bind matrix buffer of size %d, got %d\n",ibm_expected_size,read);
	}
	memcpy(new_skeleton->inverse_bind_mats,ibm_data_buffer,read);

	for(int i=0;i<new_skeleton->bone_count;i++){
		JSONObject* bone_node = nodes_array->At(joint_nodes->At(i)->IntValue())->ObjectValue();
		JSONArray *pos_array=null,*rot_array=null,*scale_array=null,*child_array=null;
		new_skeleton->bones[i].name = cstr::new_copy(bone_node->GetString("name")->string);
		float px=0,py=0,pz=0;
		quaternion rotation;rotation.clear();
		vec3 scale = {1,1,1};

		if(bone_node->HasArray("position")){
			pos_array=bone_node->GetArray("position");
			px = pos_array->At(0)->FloatValue();
			py = pos_array->At(1)->FloatValue();
			pz = pos_array->At(2)->FloatValue();
		}
		if(bone_node->HasArray("rotation")){
			rot_array=bone_node->GetArray("rotation");
			rotation.x = rot_array->At(0)->FloatValue();
			rotation.y = rot_array->At(1)->FloatValue();
			rotation.z = rot_array->At(2)->FloatValue();
			rotation.w = rot_array->At(3)->FloatValue();
		}
		if(bone_node->HasArray("scale")){
			scale_array=bone_node->GetArray("scale");
			px = scale_array->At(0)->FloatValue();
			py = scale_array->At(1)->FloatValue();
			pz = scale_array->At(2)->FloatValue();
		}
		if(bone_node->HasArray("children")){
			child_array=bone_node->GetArray("children");
			new_skeleton->bones[i].child_count=child_array->count;
			new_skeleton->bones[i].child_indices = (int*)calloc(child_array->count,sizeof(int*));
			for(int j=0;j < child_array->count;j++){
				int child_index = child_array->At(j)->IntValue();
				new_skeleton->bones[i].child_indices[j] = child_index;
				new_skeleton->bones[child_index].parent_index = i;
			}
		}
		new_skeleton->bones[i].bind_transform.identity();
		new_skeleton->bones[i].bind_transform.transform(px,py,pz,rotation,scale);
	}
}