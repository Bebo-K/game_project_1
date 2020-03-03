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

VBO GLTFScene::BuildAccessorBuffer(int id,GLuint bufferType){
	VBO ret;
	JSONObject* accessor = gltf_data->GetArray("accessors")->At(id)->ObjectValue();
	int attrib_count = accessor->GetInt("count");
	int elements_per_attrib = 1;
	int element_type = accessor->GetInt("componentType");
	char* type = accessor->GetString("type")->string;
	if(cstr::compare(type,"SCALAR")){elements_per_attrib = 1;}
	if(cstr::compare(type,"VEC2")){elements_per_attrib = 2;}
	if(cstr::compare(type,"VEC3")){elements_per_attrib = 3;}
	if(cstr::compare(type,"VEC4")){elements_per_attrib = 4;}
	if(cstr::compare(type,"MAT2")){elements_per_attrib = 4;}
	if(cstr::compare(type,"MAT3")){elements_per_attrib = 9;}
	if(cstr::compare(type,"MAT4")){elements_per_attrib = 16;}

	int element_size = 1;
	switch(element_type){
		case 5120:element_size*=1;break;
		case 5121:element_size*=1;break;
		case 5122:element_size*=2;break;
		case 5123:element_size*=2;break;
		case 5126:element_size*=4;break;
		case 5125:element_size*=4;break;
		default:element_size*=4;break;
	}
	int attrib_size = element_size*elements_per_attrib;

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
			new_data=(byte*)calloc(attrib_size,attrib_count);
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
			memcpy(&element_data[attrib_size*indices[i]],&values_data[attrib_size*i],attrib_size);
		}
	}
	if(accessor->HasInt("byteOffset")){
		element_data = &element_data[accessor->GetInt("byteOffset")];
	}

	ret.Create(element_data,element_type,elements_per_attrib,attrib_count,bufferType);

	if(new_data != null){free(new_data);}
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

Material GLTFScene::GetMaterial(int mat_id){
	Material ret;
	if(!gltf_data->HasArray("materials")){return ret;}
	JSONArray* gltf_mats = gltf_data->GetArray("materials");
	if(mat_id < 0 || mat_id > gltf_mats->count);
	JSONObject* material = gltf_mats->At(mat_id)->ObjectValue();
	
	if(material->HasBool("doubleSided")){
		ret.cull_backface=!(material->GetBool("doubleSided"));
	}
	if(material->HasJObject("pbrMetallicRoughness")){
		JSONObject* PBRMaterial = material->GetJObject("pbrMetallicRoughness");
		JSONArray* baseColor=PBRMaterial->GetArray("baseColorFactor");	
			ret.base_color[0] = baseColor->At(0)->FloatValue();
			ret.base_color[1] = baseColor->At(1)->FloatValue();
			ret.base_color[2] = baseColor->At(2)->FloatValue();
			ret.base_color[3] = baseColor->At(3)->FloatValue();
		ret.metallic_factor=PBRMaterial->GetFloat("metallicFactor");
		ret.roughness_factor=PBRMaterial->GetFloat("roughnessFactor");
	}
	return ret;
}

char* GLTFScene::FindMeshGroupName(int group_id){
	JSONArray* nodes = gltf_data->GetArray("nodes");
	for(int i=0;i<nodes->count;i++){
		JSONObject* node = nodes->At(i)->ObjectValue();
		if(node->HasInt("mesh") && node->GetInt("mesh") == group_id){
			if(node->HasString("name")){
				return cstr::new_copy(node->GetString("name")->string);
			}
		}
	}
	return null;
}

MeshGroup* GLTFScene::GetMeshGroup(int group_id){
	if(!gltf_data->HasArray("meshes")){return nullptr;}
	JSONArray* meshes = gltf_data->GetArray("meshes");
	if(group_id < 0 || group_id >= meshes->count){return nullptr;}

	MeshGroup* ret = (MeshGroup*)calloc(1,sizeof(MeshGroup));

	JSONObject* mesh = meshes->At(group_id)->ObjectValue();
	JSONArray* primitives = mesh->GetArray("primitives");


	ret->name = FindMeshGroupName(group_id);
	if(ret->name== null){ret->name = cstr::new_copy(mesh->GetString("name")->string);}
	ret->mesh_count=primitives->count;
	ret->meshes = (Mesh*)calloc(primitives->count,sizeof(Mesh));

	for(int i=0;i<primitives->count;i++){
		Mesh* prim = &ret->meshes[i];
		JSONObject* primitive = primitives->At(i)->ObjectValue();
		JSONObject* attribs = primitive->GetJObject("attributes");

		int pos_attrib_id=attribs->GetInt("POSITION");
		JSONObject* pos_accessor = GetAccessor(pos_attrib_id);
		prim->vertex_count = pos_accessor->GetInt("count");

		JSONArray* max_array = pos_accessor->GetArray("max");
		JSONArray* min_array = pos_accessor->GetArray("min");
		AABB prim_bounds;
			prim_bounds.hi_corner.x= max_array->At(0)->FloatValue();
			prim_bounds.hi_corner.y= max_array->At(1)->FloatValue();
			prim_bounds.hi_corner.z= max_array->At(2)->FloatValue();
			prim_bounds.lo_corner.x= min_array->At(0)->FloatValue();
			prim_bounds.lo_corner.y= min_array->At(1)->FloatValue();
			prim_bounds.lo_corner.z= min_array->At(2)->FloatValue();
		ret->bounds.Union(prim_bounds);

		prim->vertex=BuildAccessorBuffer(pos_attrib_id,GL_ARRAY_BUFFER);
		if(attribs->HasInt("NORMAL")){
			prim->normal=BuildAccessorBuffer(attribs->GetInt("NORMAL"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("TEXCOORD_0")){
			prim->texcoord_0=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_0"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("JOINTS_0")){
			prim->bone_0_index=BuildAccessorBuffer(attribs->GetInt("JOINTS_0"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("WEIGHTS_0")){
			prim->bone_0_weight=BuildAccessorBuffer(attribs->GetInt("WEIGHTS_0"),GL_ARRAY_BUFFER);}
		//if(attribs->HasInt("TEXCOORD_1")){
		//	mesh->texcoord_0_buffer=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_1"));}
		
		prim->mat = GetMaterial(primitive->GetInt("material"));
		if(primitive->HasInt("indices")){
			int index_accessor_id=primitive->GetInt("indices"); 
			prim->index = BuildAccessorBuffer(index_accessor_id,GL_ELEMENT_ARRAY_BUFFER);
			prim->vertex_count = GetAccessor(index_accessor_id)->GetInt("count");
		}
	}
	return ret;
}

Skeleton* GLTFScene::GetSkeleton(int skeleton_id){
	if(!gltf_data->HasArray("skins")){return nullptr;}
	JSONArray* skins = gltf_data->GetArray("skins");
	if(skeleton_id < 0 || skeleton_id >= skins->count){return nullptr;}

	JSONArray* nodes_array = gltf_data->GetArray("nodes");

	Skeleton* ret = (Skeleton*)calloc(1,sizeof(Skeleton));

	JSONObject* skin = skins->At(skeleton_id)->ObjectValue();
	JSONArray* joint_nodes = skin->GetArray("joints");

	//char* skeleton_name = skin->GetString("name")->string;
	ret->AllocateBoneCount(joint_nodes->count);

	int ibm_data_buffer_id = GetAccessor(skin->GetInt("inverseBindMatrices"))->GetInt("bufferView");
	int read =0;
	byte* ibm_data_buffer=GetBufferViewData(ibm_data_buffer_id,&read);
	int ibm_expected_size = sizeof(mat4)*ret->bone_count;
	if(ibm_expected_size != read){
		logger::exception("Expected inverse bind matrix buffer of size %d, got %d\n",ibm_expected_size,read);
	}
	memcpy(ret->inverse_bind_mats,ibm_data_buffer,read);
	for(int i=0;i<ret->bone_count;i++){
		ret->inverse_bind_mats[i].transpose();
	}

	float px=0,py=0,pz=0;
	quaternion rotation;
	vec3 scale = {1,1,1};
	for(int i=0;i<ret->bone_count;i++){
		JSONObject* bone_node = nodes_array->At(joint_nodes->At(i)->IntValue())->ObjectValue();
		JSONArray *pos_array=null,*rot_array=null,*scale_array=null,*child_array=null;
		Bone* bone = &ret->bones[i];
		bone->name = cstr::new_copy(bone_node->GetString("name")->string);
		
		px=0;py=0;pz=0;
		scale.set(1,1,1);
		rotation.clear();
		
		if(bone_node->HasArray("translation")){
			pos_array=bone_node->GetArray("translation");
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
			scale.x = scale_array->At(0)->FloatValue();
			scale.y = scale_array->At(1)->FloatValue();
			scale.z = scale_array->At(2)->FloatValue();
		}
		if(bone_node->HasArray("children")){
			child_array=bone_node->GetArray("children");
			ret->bones[i].child_count=child_array->count;
			ret->bones[i].child_indices = (int*)calloc(child_array->count,sizeof(int*));
			for(int j=0;j < child_array->count;j++){
				int child_node_index = child_array->At(j)->IntValue();
				int child_bone_index= -1;
				for(int k=0;k<joint_nodes->count;k++){
					if(joint_nodes->At(k)->IntValue() == child_node_index){
						child_bone_index=k;
					}
				}
				ret->bones[i].child_indices[j] = child_bone_index;
				ret->bones[child_bone_index].parent_index = i;
			}
		}
		bone->bind_transform.identity();
		bone->bind_transform.transform(px,py,pz,rotation,scale);
	}
	//ret->SolvePoseOrder();
	return ret;
}

Model* GLTFScene::LoadAsModel(char* name){
	Model* ret = new Model();
	base_name=name;
	ret->name = cstr::new_copy(name);
	ret->mesh_group_count=0;

	PointerArray model_meshes(1);
	int skeleton_id =-1;

	JSONArray* nodes_array = gltf_data->GetArray("nodes");
	JSONObject* node =nullptr;
	for(int i=0;i<nodes_array->count;i++){
		node = nodes_array->At(i)->ObjectValue();
		if(node->HasInt("mesh")){
			if(node->HasInt("skin")){
				int mesh_skeleton_id = node->GetInt("skin");
				if(skeleton_id < 0){
					skeleton_id = mesh_skeleton_id;
					ret->skeleton=GetSkeleton(skeleton_id);
				}
				else if(skeleton_id != mesh_skeleton_id){
					logger::warn("Skipping mesh group %d using a different skeleton(id %d)\n",i,skeleton_id);
					continue;
				}
			}
			ret->mesh_group_count++;
			model_meshes.Add((byte*)GetMeshGroup(node->GetInt("mesh")));
		}
	}

	ret->mesh_groups = (MeshGroup*)calloc(ret->mesh_group_count,sizeof(MeshGroup));
	for(int i=0;i<ret->mesh_group_count;i++){
		MeshGroup* group =(MeshGroup*)model_meshes.Get(i);
		memcpy(&ret->mesh_groups[i],group,sizeof(MeshGroup));
		ret->bounds.Union(group->bounds);
		free(group);
	}

	return ret;
}