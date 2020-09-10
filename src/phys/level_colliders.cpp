
#include "../log.h"
#include "level_colliders.h"
#include "collision_types.h"
#include "coll_triangle_handler.h"

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

int GetIndex(byte* indices,int vertex,int index_sizeof){
    return(index_sizeof == 2)?((short*)indices)[vertex]:((int*)indices)[vertex];
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
        float* mesh_verts = (float*)calloc(current_mesh->vertex_count*3,sizeof(float));

        if(group->meshes[i].index.Valid()){
            int index_sizeof;
            switch(current_mesh->index.element_type){
                case GL_BYTE:index_sizeof=1;break;
                case GL_SHORT:index_sizeof=2;break;
                case GL_UNSIGNED_SHORT:index_sizeof=2;break;
                case GL_INT:index_sizeof=2;break;
                case GL_UNSIGNED_INT:index_sizeof=2;break;
                default: index_sizeof=4;break;
            }
            
            byte* indices = (byte*)calloc(group->meshes[i].vertex_count,sizeof(int));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,current_mesh->index.buffer_id);
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,current_mesh->vertex_count * index_sizeof, indices);
            int gl_err = glGetError();
            if(gl_err != 0){logger::warn("GL error building collision mesh: index buffer error %d",&gl_err);}
            
            int max_index=0;
            for(int j=0;j< current_mesh->vertex_count;j++){
                int index =GetIndex(indices,j,index_sizeof);
                if(max_index < index){max_index=index;}
            }
            
            float* non_indexed_verts = (float*)calloc(current_mesh->vertex_count,sizeof(float)*3);

            glBindBuffer(GL_ARRAY_BUFFER,current_mesh->vertex.buffer_id);
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, (max_index+1) * sizeof(float) * 3, non_indexed_verts);
            
            gl_err = glGetError();
            if(gl_err != 0){
                logger::warn("GL error building collision mesh: vertex buffer error %d\n",&gl_err);
            }

            for(int j=0;j< current_mesh->vertex_count;j++){
                int index = GetIndex(indices,j,index_sizeof);
                logger::info("(J:%d) Index: %d\n",j,index);
                mesh_verts[j*3] =   non_indexed_verts[index*3];
                mesh_verts[j*3+1] = non_indexed_verts[index*3+1];
                mesh_verts[j*3+2] = non_indexed_verts[index*3+2];
                logger::info("      {%f, %f, %f }\n",mesh_verts[j*3],mesh_verts[j*3+1],mesh_verts[j*3+2]);
            }
            
            free(indices);
            free(non_indexed_verts);
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

CollisionList* CollisionMesh::CheckCollisions(Entity* e,vec3 step_pos){
    if(e->phys_data == null)return null;

    CollisionList* ret =null;

    if(!bounds.ContainsCircle_XZ(step_pos,e->phys_data->world_hitsphere.radius))return ret;
		
    Triangle triangle;

    for(int t=0;t < vertex_count*3;t+=9){
        triangle.verts[0].x = vertices[t];
        triangle.verts[0].y = vertices[t+1];
        triangle.verts[0].z = vertices[t+2];
        triangle.verts[1].x = vertices[t+3];
        triangle.verts[1].y = vertices[t+4];
        triangle.verts[1].z = vertices[t+5];
        triangle.verts[2].x = vertices[t+6];
        triangle.verts[2].y = vertices[t+7];
        triangle.verts[2].z = vertices[t+8];
        
        triangle.edges[0] = triangle.verts[1]-triangle.verts[0];
        triangle.edges[1] = triangle.verts[2]-triangle.verts[0];
        triangle.edges[2] = triangle.verts[1]-triangle.verts[2];
        triangle.face.SetFromPointNormal(triangle.verts[0],triangle.edges[0].normalized().cross(triangle.edges[1].normalized()));
        
        if( triangle.face.normal.length_sqr() < 0.25){
            continue;
        }//Skip zero-sized triangles. Check for duplicate verts!

        ret=CollisionList::Append(ret,TriangleHandler::DoCollision(&surface,e,step_pos, e->phys_data->world_hitsphere,triangle));
    }	
    return ret;
}

void CollisionMesh::CheckOOB(Entity* e){
    if(e->phys_data == null)return;
    vec3 step_pos = {e->x,e->y,e->z};

    if(!bounds.ContainsCircle_XZ(step_pos,e->phys_data->world_hitsphere.radius))return;
		
    Triangle triangle;

    for(int t=0;t < vertex_count;t+=9){
        triangle.verts[0].x = vertices[t];
        triangle.verts[0].y = vertices[t+1];
        triangle.verts[0].z = vertices[t+2];
        triangle.verts[1].x = vertices[t+3];
        triangle.verts[1].y = vertices[t+4];
        triangle.verts[1].z = vertices[t+5];
        triangle.verts[2].x = vertices[t+6];
        triangle.verts[2].y = vertices[t+7];
        triangle.verts[2].z = vertices[t+8];
        
        triangle.edges[0] = triangle.verts[1]-triangle.verts[0];
        triangle.edges[1] = triangle.verts[2]-triangle.verts[0];
        triangle.edges[2] = triangle.verts[1]-triangle.verts[2];
        triangle.face.SetFromPointNormal(triangle.verts[0],triangle.edges[0].normalized().cross(triangle.edges[1].normalized()));
        
        TriangleHandler::CheckIfOOB(e,step_pos,triangle);
    }
}

CollisionList* HeightMap::CheckCollisions(Entity* e,vec3 step_pos){
    return nullptr;


}