#include <game_project_1/io/gltf.hpp>
#include <game_project_1/io/asset_manager.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/types/data_types.hpp>
#include <game_project_1/types/list.hpp>

GLTFScene::GLTFScene(Stream* model_stream) :binary_buffers(1){
	if(model_stream->error)return;
	uint32 magic_number;
	model_stream->peek(&magic_number,4);
	if(magic_number == GLB_MAGIC_NUMBER){LoadAsGLB(model_stream);}
	else{LoadAsGLTF(model_stream);}
}

GLTFScene::~GLTFScene(){
	delete gltf_data;
	for(byte* buffer:binary_buffers){free(buffer);}
	binary_buffers.Clear();
}

void GLTFScene::LoadAsGLB(Stream* glb_stream){
	uint32 magic_number;
	uint32 version;
	uint32 file_length;
	
	uint32 chunk_length;
	uint32 chunk_id;
	byte* data;
	
	glb_stream->read(&magic_number,4);
	glb_stream->read(&version,4);
	glb_stream->read(&file_length,4);

	while((uint32)glb_stream->amount_read < file_length){
		glb_stream->read(&chunk_length,4);
		glb_stream->read(&chunk_id,4);
		data = (byte*)malloc(chunk_length);
		glb_stream->read(data,chunk_length);
		
		switch(chunk_id){
			case JSON_CHUNK:{
				JSONParser parser((char*)data,chunk_length);
				gltf_data = parser.Parse();
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

void GLTFScene::LoadAsGLTF(Stream* gltf_stream){
	byte* gltf_json = (byte*)malloc(gltf_stream->length);
	gltf_stream->read(gltf_json,gltf_stream->length);

	JSONParser parser = JSONParser((char*)gltf_json,gltf_stream->length);
	gltf_data = parser.Parse();

	JSONArray* buffers = gltf_data->GetArray("buffers");
	for(int i=0;i< buffers->count;i++){
		if(buffers->At(i)->ObjectValue()->HasString("uri")){
			JSONString* buffer_uri = buffers->At(i)->ObjectValue()->GetString("uri");
			Stream* buffer_stream = AssetManager::Model(buffer_uri->string);
			binary_buffers.Add(buffer_stream->readAll());
			delete buffer_stream;
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

float* GLTFScene::BuildAccessorFloatArray(int id,int* count){
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
		element_data=GetBufferViewData(element_buffer,nullptr);
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

	float* ret = (float*)calloc(attrib_count*elements_per_attrib,sizeof(float));
	switch(element_type){
		case 5126:
			memcpy(ret,element_data,sizeof(float)*attrib_count*elements_per_attrib);
			if(count != nullptr){*count = attrib_count*elements_per_attrib;}
			break;
		default:
			logger::exception("GLTF.BuildAccessorFloatArray -> Unimplemented accessor for type %d. Tell Bebo to fix it!\n",element_type);
			return nullptr;
	}
	return ret;
}

int* GLTFScene::BuildAccessorIntArray(int id,int* count){
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

	int* ret = (int*)calloc(attrib_count*elements_per_attrib,sizeof(int));
	switch(element_type){
		case 5120://CHAR
			for(int i=0;i<attrib_count*elements_per_attrib;i++){ret[i] = (char)element_data[i];}
			break;
		case 5121://UCHAR
			for(int i=0;i<attrib_count*elements_per_attrib;i++){ret[i] = (unsigned char)element_data[i];}
			break;
		case 5122://SHORT
			for(int i=0;i<attrib_count*elements_per_attrib;i++){ret[i] = ((short*)element_data)[i];}
			break;
		case 5123://USHORT
			for(int i=0;i<attrib_count*elements_per_attrib;i++){ret[i] = ((unsigned short*)element_data)[i];}
			break;
		case 5125://UINT
			for(int i=0;i<attrib_count*elements_per_attrib;i++){ret[i] =((unsigned int*)element_data)[i];}
			break;
		default:
			logger::exception("GLTF.BuildAccessorFloatArray -> Unimplemented accessor for type %d. Tell Bebo to fix it!\n",element_type);
			return nullptr;
	}
	if(count != nullptr){*count = attrib_count*elements_per_attrib;}
	return ret;
}

byte* GLTFScene::GetBufferViewData(int buffer_view_id,int* len){
	JSONArray* bufferviews = gltf_data->GetArray("bufferViews");
	JSONObject* bufferview = bufferviews->At(buffer_view_id)->ObjectValue();
	int buffer_id= bufferview->GetInt("buffer");
	int offset= bufferview->GetInt("byteOffset");
	if(len != null){*len = bufferview->GetInt("byteLength");}
	byte* buffer = binary_buffers[buffer_id];
	return &buffer[offset];
}

Texture GLTFScene::GetTexture(int tex_id){
	Texture ret = TextureManager::DefaultTexture();

	if(!gltf_data->HasArray("textures")){return ret;}
	JSONArray* textures = gltf_data->GetArray("textures");
	if(tex_id < 0 || tex_id > textures->count){return ret;}
	JSONObject* tex = textures->At(tex_id)->ObjectValue();
	int image_id = tex->GetInt("source");
	//TODO: get texture sampling info from tex->GetInt("sampler");

	if(!gltf_data->HasArray("images")){return ret;}
	JSONArray* images = gltf_data->GetArray("images");
	if(image_id < 0 || image_id > images->count){return ret;}
	JSONObject* image = images->At(image_id)->ObjectValue();

	if(image->HasString("uri")){
		ret = TextureManager::Get(image->GetString("uri")->string);
	}
	else if(image->HasInt("bufferView")){
		if(image->HasString("mimeType")){
			if(!cstr::compare("image/png",image->GetString("mimeType")->string)){
				logger::exception("Texture image type not supported: %s model will not load correctly!\n",image->GetString("mimeType")->string);
			}
		}
		Image newImage(GetBufferViewData(image->GetInt("bufferView"),null));
		ret = TextureManager::MapToAtlas(&newImage);
	}
	return ret;
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
		if(PBRMaterial->HasArray("baseColorFactor")){
			JSONArray* baseColor=PBRMaterial->GetArray("baseColorFactor");	
				ret.base_color[0] = baseColor->At(0)->FloatValue();
				ret.base_color[1] = baseColor->At(1)->FloatValue();
				ret.base_color[2] = baseColor->At(2)->FloatValue();
				ret.base_color[3] = baseColor->At(3)->FloatValue();
		}
		if(PBRMaterial->HasJObject("baseColorTexture")){
			JSONObject* baseColorTexture=PBRMaterial->GetJObject("baseColorTexture");
				int texture_index = baseColorTexture->GetInt("index");
				ret.texture = GetTexture(texture_index);
				//TODO: Multiple texcoord loading
				//int texture_layer = baseColorTexture->GetInt("texCoord");
		}
		ret.metallic_factor=(PBRMaterial->HasFloat("metallicFactor"))?PBRMaterial->GetFloat("metallicFactor") : 0;
		ret.roughness_factor=(PBRMaterial->HasFloat("roughnessFactor"))?PBRMaterial->GetFloat("roughnessFactor") : 0;
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

void GLTFScene::GetMeshGroup(MeshGroup* group, int group_id){
	if(!gltf_data->HasArray("meshes")){return;}
	JSONArray* meshes = gltf_data->GetArray("meshes");
	if(group_id < 0 || group_id >= meshes->count){return;}

	JSONObject* mesh = meshes->At(group_id)->ObjectValue();
	JSONArray* primitives = mesh->GetArray("primitives");

	group->name = FindMeshGroupName(group_id);
	if(group->name== null){group->name = cstr::new_copy(mesh->GetString("name")->string);}
	group->meshes.Allocate(primitives->count);

	Mesh* prim = nullptr;
	for(int i=0;i<primitives->count;i++){
		prim = group->meshes[i];
		JSONObject* primitive = primitives->At(i)->ObjectValue();
		JSONObject* attribs = primitive->GetJObject("attributes");

		int pos_attrib_id=attribs->GetInt("POSITION");
		JSONObject* pos_accessor = GetAccessor(pos_attrib_id);
		prim->vertex_count = pos_accessor->GetInt("count");
		prim->tri_count = prim->vertex_count/3;

		JSONArray* max_array = pos_accessor->GetArray("max");
		JSONArray* min_array = pos_accessor->GetArray("min");
		AABB prim_bounds;
			prim_bounds.hi_corner.x= max_array->At(0)->FloatValue();
			prim_bounds.hi_corner.y= max_array->At(1)->FloatValue();
			prim_bounds.hi_corner.z= max_array->At(2)->FloatValue();
			prim_bounds.lo_corner.x= min_array->At(0)->FloatValue();
			prim_bounds.lo_corner.y= min_array->At(1)->FloatValue();
			prim_bounds.lo_corner.z= min_array->At(2)->FloatValue();
		group->bounds.Union(prim_bounds);

		prim->vertex=BuildAccessorBuffer(pos_attrib_id,GL_ARRAY_BUFFER);
		if(attribs->HasInt("NORMAL")){prim->normal=BuildAccessorBuffer(attribs->GetInt("NORMAL"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("COLOR_0")){prim->vertex_colors=BuildAccessorBuffer(attribs->GetInt("COLOR_0"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("TEXCOORD_0")){prim->texcoord_0=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_0"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("JOINTS_0")){prim->bone_0_index=BuildAccessorBuffer(attribs->GetInt("JOINTS_0"),GL_ARRAY_BUFFER);}
		if(attribs->HasInt("WEIGHTS_0")){prim->bone_0_weight=BuildAccessorBuffer(attribs->GetInt("WEIGHTS_0"),GL_ARRAY_BUFFER);}
		//if(attribs->HasInt("TEXCOORD_1")){mesh->texcoord_0_buffer=BuildAccessorBuffer(attribs->GetInt("TEXCOORD_1"));}
		if(primitive->HasInt("material")){prim->mat = GetMaterial(primitive->GetInt("material"));}
		else{
			prim->mat.base_color[0]=prim->mat.base_color[1]=prim->mat.base_color[2]=prim->mat.base_color[3]=1.0f;
			prim->mat.texture=TextureManager::DefaultTexture();
			prim->mat.metallic_factor=0;
			prim->mat.roughness_factor=0;
			prim->mat.cull_backface=false;
		}
		if(primitive->HasInt("indices")){
			int index_accessor_id=primitive->GetInt("indices"); 
			prim->index = BuildAccessorBuffer(index_accessor_id,GL_ELEMENT_ARRAY_BUFFER);
			int index_count = GetAccessor(index_accessor_id)->GetInt("count");
			prim->tri_count = index_count/3;
			if(index_count % 3 != 0){
				logger::warn("Non-Triangulated Mesh: %s has an index buffer length that is not a multiple of 3: %d", group->name, index_count);
			}
		}
		prim->Init();
	}
}

MeshCollider* GLTFScene::GetMeshGroupAsCollider(char* group_name){
	if(!gltf_data->HasArray("meshes")){return nullptr;}
	JSONArray* meshes = gltf_data->GetArray("meshes");

	MeshCollider* collider = nullptr;
	JSONArray* primitives = nullptr;

	for(int i=0;i< meshes->count;i++){
		char* mesh_group_name = FindMeshGroupName(i);
		if(cstr::compare(group_name,mesh_group_name)){
			JSONObject* mesh = meshes->At(i)->ObjectValue();
			primitives = mesh->GetArray("primitives");
			collider = new MeshCollider();
			collider->name = cstr::new_copy(group_name);
		}
	}
	if(primitives == nullptr){return nullptr;}

	collider->tri_count=0;
	vec3** primitive_verts = new vec3*[primitives->count];
	int* primitive_tri_count = new int[primitives->count];

	for(int i=0;i<primitives->count;i++){
		JSONObject* primitive = primitives->At(i)->ObjectValue();
		JSONObject* attribs = primitive->GetJObject("attributes");

		int pos_attrib_id=attribs->GetInt("POSITION");
		JSONObject* pos_accessor = GetAccessor(pos_attrib_id);
		int primitive_vert_count = pos_accessor->GetInt("count");

		JSONArray* max_array = pos_accessor->GetArray("max");
		JSONArray* min_array = pos_accessor->GetArray("min");
		AABB prim_bounds;
			prim_bounds.hi_corner.x= max_array->At(0)->FloatValue();
			prim_bounds.hi_corner.y= max_array->At(1)->FloatValue();
			prim_bounds.hi_corner.z= max_array->At(2)->FloatValue();
			prim_bounds.lo_corner.x= min_array->At(0)->FloatValue();
			prim_bounds.lo_corner.y= min_array->At(1)->FloatValue();
			prim_bounds.lo_corner.z= min_array->At(2)->FloatValue();
		collider->bounds.Union(prim_bounds);

		int primitive_float_vert_count =0;
		vec3* raw_verts = (vec3*)BuildAccessorFloatArray(pos_attrib_id,&primitive_float_vert_count);
		primitive_vert_count = primitive_float_vert_count/3;

		if(primitive->HasInt("indices")){
			int index_accessor_id=primitive->GetInt("indices"); 
			int index_count=0;
			int* indices = BuildAccessorIntArray(index_accessor_id,nullptr);
			index_count = GetAccessor(index_accessor_id)->GetInt("count");
			if(index_count % 3 != 0){
				logger::warn("Non-Triangulated Mesh: %s has an index buffer length that is not a multiple of 3: %d", collider->name, index_count);
			}
			primitive_verts[i] = new vec3[index_count];
			primitive_tri_count[i] = index_count/3;
			for(int vert=0;vert<index_count;vert++){
				primitive_verts[i][vert] = raw_verts[indices[vert]];
			}
			free(raw_verts);
		}
		else{
			if(primitive_vert_count % 9 != 0){
				logger::warn("Non-Triangulated Mesh: %s has an vertex buffer length that is not a multiple of 9: %d", collider->name, primitive_vert_count);
			}
			primitive_verts[i]=raw_verts;
			primitive_tri_count[i] = primitive_vert_count/9;
			raw_verts=nullptr;
		}
		collider->tri_count+= primitive_tri_count[i];
	}

	collider->tris = (Triangle*)calloc(collider->tri_count,sizeof(Triangle));
	int tri_index=0;
	for(int i=0;i<primitives->count;i++){
		for(int j=0;j<primitive_tri_count[i];j++){
			collider->tris[tri_index].SetFromVertices((float*)(&primitive_verts[i][j*3]));
			tri_index++;
		}
		free(primitive_verts[i]);
	}
	free(primitive_verts);
	free(primitive_tri_count);
	return collider;
}


Skeleton* GLTFScene::GetSkeleton(int skeleton_id){
	if(!gltf_data->HasArray("skins")){return nullptr;}
	JSONArray* skins = gltf_data->GetArray("skins");
	if(skeleton_id < 0 || skeleton_id >= skins->count){return nullptr;}

	JSONArray* nodes_array = gltf_data->GetArray("nodes");
	JSONObject* skin = skins->At(skeleton_id)->ObjectValue();
	JSONArray* joint_nodes = skin->GetArray("joints");

	Skeleton* ret = new Skeleton(joint_nodes->count);

	int ibm_data_buffer_id = GetAccessor(skin->GetInt("inverseBindMatrices"))->GetInt("bufferView");
	int read =0;
	byte* ibm_data_buffer=GetBufferViewData(ibm_data_buffer_id,&read);
	int ibm_expected_size = sizeof(mat4)*ret->bones.length;
	if(ibm_expected_size != read){
		logger::exception("Expected inverse bind matrix buffer of size %d, got %d\n",ibm_expected_size,read);
	}
	memcpy(ret->inverse_bind_mats,ibm_data_buffer,read);
	for(int i=0;i<ret->bones.length;i++){
		ret->inverse_bind_mats[i].transpose();
	}

	float px=0,py=0,pz=0;
	quaternion rotation;
	vec3 scale = {1,1,1};
	for(int i=0;i<ret->bones.length;i++){
		JSONObject* bone_node = nodes_array->At(joint_nodes->At(i)->IntValue())->ObjectValue();
		JSONArray *pos_array=null,*rot_array=null,*scale_array=null,*child_array=null;
		Bone* bone = ret->bones[i];
		ret->SetBoneName(i,bone_node->GetString("name")->string);
		
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
			for(int j=0;j < child_array->count;j++){
				int child_node_index = child_array->At(j)->IntValue();
				int child_bone_index= -1;
				for(int k=0;k<joint_nodes->count;k++){
					if(joint_nodes->At(k)->IntValue() == child_node_index){
						child_bone_index=k;
					}
				}
				ret->bones[child_bone_index]->parent_index = i;
			}
		}
		bone->bind_transform.identity();
		bone->bind_transform.transform(px,py,pz,rotation,scale);
	}
	return ret;
}

void GLTFScene::LoadAnimation(int animation_id,Skeleton* target){
	JSONObject* animation = gltf_data->GetArray("animations")->At(animation_id)->ObjectValue();
	JSONArray* channels = animation->GetArray("channels");

	Animation* dest = target->animations[animation_id];

	dest->SetName(animation->GetString("name")->string);
	dest->SetChannelCount(channels->count);
	dest->length= 0.0f;

	for(int i=0; i < dest->channel_count; i++){
		AnimationChannel* channel = &dest->channels[i];
		JSONObject* gltf_channel =  channels->At(i)->ObjectValue();
		JSONObject* channel_target = gltf_channel->GetJObject("target");
		
		int target_node_id = channel_target->GetInt("node");
		JSONObject* target_node = gltf_data->GetArray("nodes")->At(target_node_id)->ObjectValue();
		char* target_node_name = target_node->GetString("name")->string;
		char* target_type = channel_target->GetString("path")->string;

		channel->target.object_name=null;
		for(int b=0;b<target->bones.length;b++){
			if(cstr::compare(target_node_name,target->bones[b]->name)){
				channel->target.object_name=target->bones[b]->name;
				break;
			}
		}
		if(channel->target.object_name==null){
			continue;
			//TODO: Non bone channels
			//channel->target.object_name=cstr::new_copy(target_node_name);
		}

		if(cstr::compare(target_type,"translation")){
			channel->target.value_type=AnimationType::TRANSLATION;
			channel->target.num_values=3;
		}
		else if(cstr::compare(target_type,"rotation")){
			channel->target.value_type=AnimationType::ROTATION;
			channel->target.num_values=4;
		}
		else if(cstr::compare(target_type,"scale")){
			channel->target.value_type=AnimationType::SCALE;
			channel->target.num_values=3;
		}
		else if(cstr::compare(target_type,"weight")){
			channel->target.value_type=AnimationType::WEIGHT;
			channel->target.num_values=1;
		}
		else{
			continue;
		}

		int sampler_id = gltf_channel->GetInt("sampler");
		JSONObject* sampler = animation->GetArray("samplers")->At(sampler_id)->ObjectValue();
		int sampler_time_accessor = sampler->GetInt("input");
		int sampler_value_accessor = sampler->GetInt("output");

		char* animation_type = sampler->GetString("interpolation")->string;
		if(cstr::compare(animation_type,"LINEAR")){
			channel->interpolate_mode = AnimationInterpolateMode::LINEAR;
		}
		else if(cstr::compare(animation_type,"STEP")){
			channel->interpolate_mode = AnimationInterpolateMode::STEP;
		}
		else if(cstr::compare(animation_type,"CUBICSPLINE")){
			channel->interpolate_mode = AnimationInterpolateMode::CUBICORSOMETHING;
		}else{
			continue;
		}

		channel->keyframe_times = BuildAccessorFloatArray(sampler_time_accessor,
			&channel->keyframe_count);
		channel->keyframe_values = BuildAccessorFloatArray(sampler_value_accessor,nullptr);
		
		if(channel->keyframe_times[channel->keyframe_count-1] > dest->length){
			dest->length = channel->keyframe_times[channel->keyframe_count-1];
		}
	}
}

void GLTFScene::GetModel(ModelData* model){
	int mesh_group_count=0;

	JSONArray* nodes_array = gltf_data->GetArray("nodes");
	int* mesh_ids = new int[nodes_array->count];//can't have more meshes than nodes.

	JSONObject* node =nullptr;
	int model_skeleton_id = -1;
	for(int i=0;i<nodes_array->count;i++){
		node = nodes_array->At(i)->ObjectValue();
		if(node->HasInt("mesh")){
			if(node->HasInt("skin")){
				int new_skeleton_id = node->GetInt("skin");
				if(model_skeleton_id >= 0 && model_skeleton_id != new_skeleton_id){
					logger::exception("Multiple skeletons per model not supported. Only skeleton node %d will be loaded.",model_skeleton_id);
					continue;
				}
				if(model_skeleton_id == new_skeleton_id){/*skeleton already loaded*/}
				else{
					model_skeleton_id=new_skeleton_id;
					model->skeleton=GetSkeleton(model_skeleton_id);
				}
			}
			mesh_ids[mesh_group_count] = node->GetInt("mesh");
			mesh_group_count++;
		}
	}
	model->mesh_groups.Allocate(mesh_group_count);
	for(int i=0;i<mesh_group_count;i++){
		GetMeshGroup(model->mesh_groups[i],mesh_ids[i]);
		model->bounds.Union(model->mesh_groups[i]->bounds);
	}
	if(gltf_data->HasArray("animations") && model->skeleton != nullptr){
		int anim_count = gltf_data->GetArray("animations")->count;
		model->skeleton->animations.Allocate(anim_count);
		for(int i=0;i<anim_count;i++){
			LoadAnimation(i,model->skeleton);
		}
	}

	free(mesh_ids);
}

ModelData* GLTFScene::Load(){
	ModelData* ret = new ModelData();
	GetModel(ret);
	return ret;
}

void GLTFScene::LoadIn(ModelData* dest){
	ModelData* ret = new(dest)ModelData();
	GetModel(ret);
}
