#include "texture.h"
#include "../io/log.h"

AssociativeArray texture_atlases(2);
AssociativeArray cached_textures(8);
GLuint current_atlas_id =-1;
int atlas_x=0,atlas_y=0,atlas_h=0;

Texture::Texture() {
	atlas_id = -1;
	tex_coords = {0.0f,0.0f,1.0f,1.0f};
	width_px=height_px=0;
}

TextureRectangle Texture::GetSubTexture(int  x,int y,int w,int h) {
	TextureRectangle ret = {0,0,0,0};
	
	ret.x = tex_coords.x + (tex_coords.w/width_px) * x;
	ret.y = tex_coords.y + (tex_coords.h/height_px) * h;
	ret.w = (tex_coords.w/width_px) * h;
	ret.h = (tex_coords.h/height_px) * w;
	return ret;
}

void TextureManager::Init(){
    CreateAtlas();
}
    
void TextureManager::CreateAtlas() {
    glGenTextures(1,&current_atlas_id);
    Image* img = new Image(ATLAS_SIZE,ATLAS_SIZE);
    texture_atlases.Add((int)current_atlas_id,(byte*)img);
    atlas_x = 0;
    atlas_y = 0;
    atlas_h = 0;
}

void TextureManager::SubmitAtlas(){		
    Image* atlas = (Image*)texture_atlases.Get(current_atlas_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,current_atlas_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ATLAS_SIZE,ATLAS_SIZE,0,
            GL_RGBA,GL_UNSIGNED_BYTE,atlas->image_data);
}

Texture TextureManager::Get(char* texname){
    Texture* cache_pointer = (Texture*)cached_textures.StrGet(texname);
    if(cache_pointer != null) {
        return *cache_pointer;			
    }
    Texture tex = Texture();
    Image* texture_image = new Image(texname);
    if(texture_image == null || texture_image->image_data==null){
        logger::exception("Texture image not found: %s ",texname);
        return tex;
    }
    tex.width_px = texture_image->width;
    tex.height_px = texture_image->height;

    Image* atlas = (Image*)texture_atlases.Get(current_atlas_id);
    int map_pos[] = {0,0};
    if(TextureManager::IsMappable(tex.width_px,tex.height_px,map_pos)) {
        texture_image->Blit(atlas, map_pos[0], map_pos[1]);
        tex.atlas_id = current_atlas_id;
        tex.tex_coords.x = (float)map_pos[0]/ATLAS_SIZE;
        tex.tex_coords.y = (float)map_pos[1]/ATLAS_SIZE;
        tex.tex_coords.w = (float)tex.width_px/ATLAS_SIZE;
        tex.tex_coords.h = (float)tex.height_px/ATLAS_SIZE;
        SubmitAtlas();
    }
    //if the image is unmappable, make the texture it's own atlas.
    else if(tex.width_px > ATLAS_SIZE || tex.height_px > ATLAS_SIZE) {
        glGenTextures(1,&tex.atlas_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex.atlas_id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, tex.width_px,tex.height_px,0,
                GL_RGBA,GL_UNSIGNED_BYTE,texture_image->image_data);

        tex.tex_coords.x =0;
        tex.tex_coords.y =0;
        tex.tex_coords.w =1;
        tex.tex_coords.h =1;
        texture_atlases.Add((int)tex.atlas_id,(byte*)texture_image);
    }
    else{//start a new atlas
        CreateAtlas();
        atlas = (Image*)texture_atlases.Get(current_atlas_id);
        texture_image->Blit(atlas,0,0);

        tex.atlas_id = current_atlas_id;
        tex.tex_coords.x = 0;
        tex.tex_coords.y = 0;
        tex.tex_coords.w = (float)tex.width_px/ATLAS_SIZE;
        tex.tex_coords.h = (float)tex.height_px/ATLAS_SIZE;
        
        atlas_x = tex.width_px;
        atlas_y = 0;
        atlas_h = tex.height_px;
        
        SubmitAtlas();			
    }
    Texture* cached_tex = new Texture();
        memcpy(cached_tex,&tex,sizeof(Texture));
    cached_textures.Add((byte*)texname,(byte*)cached_tex);
    return tex;
}

//try to fit an image to our texture map(lazy algorithm)
//Note: updates atlas_x/w/h after finding a map.
bool TextureManager::IsMappable(int width,int height,int* pos){
if(width <= ATLAS_SIZE){
        if(atlas_x + width <= ATLAS_SIZE && atlas_y + height <= ATLAS_SIZE){//Fits widthwise our row
            pos[0] = atlas_x;
            pos[1] = atlas_y;
            atlas_x += width;
            atlas_h = (height > atlas_h)?height:atlas_h;
            return true;
            }
        else if(atlas_y + atlas_h + height < ATLAS_SIZE){//fits below our row
            atlas_x = width;
            atlas_y += atlas_h;
            atlas_h = height;
            pos[0] = 0;
            pos[1] = atlas_y;
            return true;
        }	
    }
return false; //else doesnt fit
}	

