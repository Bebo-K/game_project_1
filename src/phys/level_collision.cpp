#include "level_collision.h"
#include "../log.h"

CollisionMesh::CollisionMesh(){
    name=null;
    vertex_count=0;
    vertices=null;
}

CollisionMesh::CollisionMesh(MeshGroup* group,CollisionSurface* coll_surface){
    SetSurface(coll_surface);
    SetVertices(group);
}

CollisionMesh::~CollisionMesh(){
    if(name != null){free(name);name=null;}
    if(vertices != null){free(vertices);vertices=null;}
}

void CollisionMesh::SetSurface(CollisionSurface* coll_surface){
    surface.type = coll_surface->type;
    surface.material = coll_surface->material;
    surface.metadata = coll_surface->metadata;
    surface.metadata_len = coll_surface->metadata_len;
}

void CollisionMesh::SetVertices(MeshGroup* group){
    name = cstr::new_copy(group->name);
    bounds=group->bounds;

    vertex_count=0;
    for(int i=0;i<group->mesh_count;i++){
        vertex_count += group->meshes[i].vertex_count;
    }

    vertices = (float*)calloc(vertex_count,sizeof(float)*3);

    int current_vert_count=0;
    for(int i=0;i<group->mesh_count;i++){
        Mesh* current_mesh = &group->meshes[i];
        float* mesh_verts = (float*)calloc(current_mesh->vertex_count,sizeof(float)*3);

        if(vertex_count += group->meshes[i].index.Valid()){
            int* indices = (int*)calloc(group->meshes[i].vertex_count,sizeof(int));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,current_mesh->index.buffer_id);
            int err = glGetError();
            if(err != 0){
                logger::warn("GL error building collision mesh: %d",&err);
            }
            int index_sizeof =2;
            switch(current_mesh->index.element_type){
                case GL_BYTE:index_sizeof=1;break;
                case GL_SHORT:index_sizeof=2;break;
                case GL_UNSIGNED_SHORT:index_sizeof=2;break;
                case GL_INT:index_sizeof=2;break;
                case GL_UNSIGNED_INT:index_sizeof=2;break;
                default: index_sizeof=4;break;
            }
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,current_mesh->vertex_count * index_sizeof, indices);
            if(index_sizeof == 2){
                short* short_indices= (short*)indices;
                for(int i=current_mesh->vertex_count-1;i>=0;i--){
                    indices[i]=(int)short_indices[i];
                }
            }
            err = glGetError();
            if(err != 0){
                logger::warn("GL error building collision mesh: index buffer error %d",&err);
            }
            int max_vertex=0;
            for(int j=0;j< current_mesh->vertex_count;j++){if(max_vertex < indices[i])max_vertex=indices[i];}
            float* indexed_verts = (float*)calloc(max_vertex,sizeof(float));

            glBindBuffer(GL_ARRAY_BUFFER,current_mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, max_vertex * sizeof(float) * 3, indexed_verts);
            
            err = glGetError();
            if(err != 0){
                logger::warn("GL error building collision mesh: vertex buffer error %d",&err);
            }

            for(int j=0;j< current_mesh->vertex_count;j++){mesh_verts[i] = indexed_verts[indices[j]];}
            free(indices);
            free(indexed_verts);
        }
        else{
            glBindBuffer(GL_ARRAY_BUFFER,current_mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, current_mesh->vertex_count * sizeof(float) * 3, mesh_verts);
        }

        memcpy(vertices,mesh_verts,current_mesh->vertex_count*sizeof(float)*3);
        current_vert_count+=group->meshes[i].vertex_count;
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


CollisionData* CollisionMesh::HandleCollision(Entity e,float delta){
    return nullptr;

}

CollisionData* HeightMap::HandleCollision(Entity e,float delta){
    return nullptr;


}