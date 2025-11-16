#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/triangle_handler.hpp>
#include <game_project_1/component/shared/physics_properties.hpp>
#include <game_project_1/component/shared/physics_state.hpp>
#include <game_project_1/io/log.hpp>

using namespace LevelCollision;

CollisionResult CollisionResult::None(){
    return {null,{0,0,0},{0,0,0},{0,0,0},0,0.0f};
}

bool CollisionResult::isNone(){return surface==null;}

void CollisionResult::Clear(){
    surface=null;
    normal={0,0,0};
    velocity_cancel={0,0,0};
    shunt={0,0,0};
    collision_case=0;
    floor_distance=0.0f;
}

Surface::Surface(){
    type = SOLID;
    material=STONE;
    exit_id=0;
    damage=0;
    metadata=null;
    metadata_len=0;
}

void Surface::SetMaterial(char* type_name,char* material_name){
    type = SOLID;
    material=STONE;
    exit_id=0;
    damage=0;
    metadata=null;
    metadata_len=0;

    if(cstr::compare_caseless(type_name,"SOLID")){type=SOLID;}
    else if(cstr::compare_caseless(type_name,"NOCLIP")){type=NOCLIP;}
    else if(cstr::compare_caseless(type_name,"TRIGGER")){type=TRIGGER;}
    else if(cstr::compare_caseless(type_name,"WATER")){type=WATER;}
    else if(cstr::compare_caseless(type_name,"DAMAGE")){type=DAMAGE;}
    else if(cstr::compare_caseless(type_name,"DEATH")){type=DEATH;}
    else{type=SOLID;}

    if(cstr::compare_caseless(material_name,"NONE")){material=NONE;}
    else if(cstr::compare_caseless(material_name,"STONE")){material=STONE;}
    else if(cstr::compare_caseless(material_name,"DIRT")){material=DIRT;}
    else if(cstr::compare_caseless(material_name,"SAND")){material=SAND;}
    else{material=STONE;}
}


MeshCollider::MeshCollider(){
    name=null;
    tri_count=0;
    tris=null;
}

MeshCollider::MeshCollider(MeshGroup* group,Surface* coll_surface){
    SetSurface(coll_surface);
    SetVertices(group);
}

MeshCollider::~MeshCollider(){
    DEALLOCATE(name)
    DEALLOCATE(tris)
}

void MeshCollider::ShallowCopy(MeshCollider* m2){
    name = m2->name;
    surface = m2->surface;
    bounds = m2->bounds;
    tri_count = m2->tri_count;
    tris=m2->tris;
}

void MeshCollider::SetSurface(Surface* coll_surface){
    surface.type = coll_surface->type;
    surface.material = coll_surface->material;
    surface.metadata = coll_surface->metadata;
    surface.metadata_len = coll_surface->metadata_len;
}

int GetIndex(byte* indices,int vertex,int index_sizeof){
    return(index_sizeof == 2)?((short*)indices)[vertex]:((int*)indices)[vertex];
}

void MeshCollider::SetVertices(MeshGroup* group){
    name = cstr::new_copy(group->name);
    bounds=group->bounds;

    tri_count=0;
    for(Mesh* mesh:group->meshes){tri_count += mesh->tri_count;}

    tris = (Triangle*)calloc(tri_count,sizeof(Triangle));

    int current_triangle=0;
    for(Mesh* mesh:group->meshes){
        float* mesh_verts = (float*)calloc(mesh->tri_count*3*3,sizeof(float));
        
        if(mesh->index.Valid()){//de-index vertices
            int index_sizeof;
            int index_count = mesh->tri_count*3;
            byte* indices = nullptr;
            float* raw_verts = nullptr;
            switch(mesh->index.element_type){
                case GL_BYTE:index_sizeof=1;break;
                case GL_SHORT:index_sizeof=2;break;
                case GL_UNSIGNED_SHORT:index_sizeof=2;break;
                case GL_INT:index_sizeof=2;break;
                case GL_UNSIGNED_INT:index_sizeof=2;break;
                default: index_sizeof=4;break;
            }
            indices = (byte*)calloc(index_count,index_sizeof);
            
            glBindVertexArray(mesh->vertex_array_id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->index.buffer_id);
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,index_count * index_sizeof, indices);
            int gl_err = glGetError();
            if(gl_err != 0){logger::warn("GL error building collision mesh: index buffer error %d\n",&gl_err);}
            
            raw_verts = (float*)calloc(mesh->vertex_count,sizeof(float)*3);
            glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertex_count * 3 * sizeof(float), raw_verts);
            gl_err = glGetError();
            if(gl_err != 0){logger::warn("GL error building collision mesh: vertex buffer error %d\n",&gl_err);}

            for(int j=0;j< index_count;j++){
                int index = GetIndex(indices,j,index_sizeof);
                mesh_verts[j*3] =   raw_verts[index*3];
                mesh_verts[j*3+1] = raw_verts[index*3+1];
                mesh_verts[j*3+2] = raw_verts[index*3+2];
            }
            
            free(indices);
            free(raw_verts);
        }
        else{//vertices are already in tri_order
            glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertex_count * sizeof(float) * 3, mesh_verts);
        }
        
        for(int t=0;t<mesh->tri_count;t++){
            tris[current_triangle+t].SetFromVertices(&mesh_verts[t*9]);
        }
        current_triangle+=mesh->tri_count;
        free(mesh_verts);
    } 
}

HeightMapCollider::HeightMapCollider(){
    samples=nullptr;
    surface.metadata_len=0;
    surface.metadata=nullptr;
}

HeightMapCollider::HeightMapCollider(float width, float height, float depth, Image* img,Surface* coll_surface){
    surface.type = coll_surface->type;
    surface.material = coll_surface->material;
    surface.metadata = coll_surface->metadata;
    surface.metadata_len = coll_surface->metadata_len;
}

HeightMapCollider MakeDeathPlane(float z_pos){
    HeightMapCollider ret;
    ret.width = 10000;
    ret.height = 10000;
    ret.depth = z_pos*2;
    ret.sample_height=ret.sample_width=1;
    ret.samples = (float*)malloc(sizeof(float));
    ((float*)ret.samples)[0] = 0.0f;

    ret.bounds.hi_corner.x = ret.width;
    ret.bounds.hi_corner.y = z_pos;
    ret.bounds.hi_corner.z = ret.height;
    ret.bounds.lo_corner.x = -ret.width;
    ret.bounds.lo_corner.y = -z_pos;
    ret.bounds.lo_corner.z = -ret.height;

    ret.surface.material = LevelCollision::Material::NONE;
    ret.surface.type= SurfaceType::DEATH;
    ret.surface.metadata_len=0;
    ret.surface.metadata=nullptr;
    return ret;
}

void MeshCollider::CheckCollisions(Entity* e,vec3 step_pos,vec3 step_velocity,CollisionResult* list){
    PhysicsProperties* phys_props = e->Get<PhysicsProperties>();
    if(!bounds.ContainsCircle_XZ(step_pos,phys_props->world_hitsphere.radius))return;
	
    for(int t=0;t < tri_count;t++){
        if(tris[t].IsZeroArea())continue;
        CollisionResult new_instance = 
            TriangleHandler::DoCollision(e,&surface,step_pos,step_velocity,phys_props->world_hitsphere,tris[t]);

        if(!new_instance.isNone()){
            for(int i=0;i<CollisionResult::MAX_PER_FRAME;i++){
                if(list[i].isNone()){list[i] = new_instance;break;}
            }
        }
    }
}

void MeshCollider::CheckOOB(Entity* e){
    vec3 step_pos = {e->x,e->y,e->z};
    PhysicsState* phys_state = e->Get<PhysicsState>();
    PhysicsProperties* phys_props = e->Get<PhysicsProperties>();
    if(!bounds.ContainsCircle_XZ(step_pos,phys_props->world_hitsphere.radius))return;
	
    for(int t=0;t < tri_count;t++){
        if(tris[t].IsZeroArea())continue;
        if(phys_state &&  TriangleHandler::CheckTriangleBounds(step_pos,tris[t])){
            phys_state->in_bounds=true;
        }
    }
}

void HeightMapCollider::CheckCollisions(Entity* e,vec3 step_pos,CollisionResult* list){}