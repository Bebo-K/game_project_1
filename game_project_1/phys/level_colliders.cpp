#include <game_project_1/phys/level_colliders.hpp>
#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/phys/coll_triangle_handler.hpp>
#include <game_project_1/component/phys_components.hpp>
#include <game_project_1/log.hpp>

CollisionMesh::CollisionMesh(){
    name=null;
    tri_count=0;
    tris=null;
}

CollisionMesh::CollisionMesh(MeshGroup* group,CollisionSurface* coll_surface){
    SetSurface(coll_surface);
    SetVertices(group);
}

CollisionMesh::~CollisionMesh(){
    if(name != null){free(name);name=null;}
    if(tris != null){free(tris);tris=null;}
}

void CollisionMesh::SetSurface(CollisionSurface* coll_surface){
    surface.type = coll_surface->type;
    surface.material = coll_surface->material;
    surface.metadata = coll_surface->metadata;
    surface.metadata_len = coll_surface->metadata_len;
}

int GetIndex(byte* indices,int vertex,int index_sizeof){
    return(index_sizeof == 2)?((short*)indices)[vertex]:((int*)indices)[vertex];
}

void CollisionMesh::SetVertices(MeshGroup* group){
    name = cstr::new_copy(group->name);
    bounds=group->bounds;

    tri_count=0;
    for(int i=0;i<group->mesh_count;i++){tri_count += group->meshes[i].tri_count;}

    tris = (Triangle*)calloc(tri_count,sizeof(Triangle));

    int current_triangle=0;
    for(int i=0;i<group->mesh_count;i++){
        Mesh* current_mesh = &group->meshes[i];
        float* mesh_verts = (float*)calloc(current_mesh->tri_count*3*3,sizeof(float));
        
        if(group->meshes[i].index.Valid()){//de-index vertices
            int index_sizeof;
            int index_count = group->meshes[i].tri_count*3;
            byte* indices = nullptr;
            float* raw_verts = nullptr;
            switch(current_mesh->index.element_type){
                case GL_BYTE:index_sizeof=1;break;
                case GL_SHORT:index_sizeof=2;break;
                case GL_UNSIGNED_SHORT:index_sizeof=2;break;
                case GL_INT:index_sizeof=2;break;
                case GL_UNSIGNED_INT:index_sizeof=2;break;
                default: index_sizeof=4;break;
            }
            indices = (byte*)calloc(index_count,index_sizeof);
            
            glBindVertexArray(group->meshes[i].vertex_array_id);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,current_mesh->index.buffer_id);
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,index_count * index_sizeof, indices);
            int gl_err = glGetError();
            if(gl_err != 0){logger::warn("GL error building collision mesh: index buffer error %d",&gl_err);}
            
            raw_verts = (float*)calloc(current_mesh->vertex_count,sizeof(float)*3);
            glBindBuffer(GL_ARRAY_BUFFER,current_mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, current_mesh->vertex_count * 3 * sizeof(float), raw_verts);
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
            glBindBuffer(GL_ARRAY_BUFFER,current_mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, current_mesh->vertex_count * sizeof(float) * 3, mesh_verts);
        }
        
        for(int t=0;t<current_mesh->tri_count;t++){
            tris[current_triangle+t].SetFromVertices(&mesh_verts[t*9]);
        }
        current_triangle+=group->meshes[i].tri_count;
        free(mesh_verts);
    } 
}

HeightMap::HeightMap(){
    samples=nullptr;
    surface.metadata_len=0;
    surface.metadata=nullptr;
}

HeightMap::HeightMap(float width, float height, float depth, Image* img,CollisionSurface coll_surface){

    surface = coll_surface;
}

HeightMap MakeDeathPlane(float z_pos){
    HeightMap ret;
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

    ret.surface.material = SurfaceMaterial::NONE;
    ret.surface.type= SurfaceType::DEATH;
    ret.surface.metadata_len=0;
    ret.surface.metadata=nullptr;
    return ret;
}

CollisionList* CollisionMesh::CheckCollisions(PhysBody* b,vec3 step_pos){
    CollisionList* ret =null;

    if(!bounds.ContainsCircle_XZ(step_pos,b->world_hitsphere.radius))return ret;
	
    for(int t=0;t < tri_count;t++){
        if(tris[t].IsZeroArea())continue;
        ret=CollisionList::Append(ret,TriangleHandler::DoCollision(b,&surface,step_pos, b->world_hitsphere,tris[t]));
    }
    return ret;
}

void CollisionMesh::CheckOOB(PhysBody* b){
    vec3 step_pos = b->GetPosition();

    if(!bounds.ContainsCircle_XZ(step_pos,b->world_hitsphere.radius))return;
	
    for(int t=0;t < tri_count;t++){
        if(tris[t].IsZeroArea())continue;
        if(TriangleHandler::CheckTriangleBounds(step_pos,tris[t])){b->SetInBounds(true);}
    }
}

CollisionList* HeightMap::CheckCollisions(PhysBody* b,vec3 step_pos){
    return nullptr;
}