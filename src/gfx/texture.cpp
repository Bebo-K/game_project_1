#include "texture.h"
#include "../log.h"

IDMap texture_atlases(2);
StringMap cached_textures(8);
GLuint current_atlas_id =-1;
GLuint empty_texture_id = -1;
int atlas_x=0,atlas_y=0,atlas_h=0;

Texture::Texture() {
	atlas_id = empty_texture_id;
	tex_coords = {0.0f,0.0f,1.0f,1.0f};
	width_px=height_px=1;
}

TextureRectangle Texture::GetSubTexture(int  x,int y,int w,int h) {
	TextureRectangle ret = {0,0,0,0};
	
	ret.x = tex_coords.x + (tex_coords.w/width_px) * x;
	ret.y = tex_coords.y + (tex_coords.h/height_px) * y;
	ret.w = (tex_coords.w/width_px) * h;
	ret.h = (tex_coords.h/height_px) * w;
	return ret;
}


void SubmitImage(Image* image){		
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,current_atlas_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->width,image->height,0,
            GL_RGBA,GL_UNSIGNED_BYTE,image->image_data);
}

Image* CreateAtlas() {
    glGenTextures(1,&current_atlas_id);
    Image* img = new Image(TextureManager::ATLAS_SIZE,TextureManager::ATLAS_SIZE);
    texture_atlases.Add((int)current_atlas_id,(byte*)img);
    atlas_x = 0;
    atlas_y = 0;
    atlas_h = 0;
    return img;
}
//try to fit an image to our texture map(lazy algorithm)
//Note: updates atlas_x/w/h after finding a map.
bool IsMappable(int width,int height,int* pos){
if(width <= TextureManager::ATLAS_SIZE){
        if(atlas_x + width <= TextureManager::ATLAS_SIZE 
        && atlas_y + height <= TextureManager::ATLAS_SIZE){//Fits widthwise our row
            pos[0] = atlas_x;
            pos[1] = atlas_y;
            atlas_x += width;
            atlas_h = (height > atlas_h)?height:atlas_h;
            return true;
            }
        else if(atlas_y + atlas_h + height < TextureManager::ATLAS_SIZE){//fits below our row
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

void TextureManager::Init(){
    SubmitImage(CreateAtlas());

    unsigned char empty_image[] = {255,255,255,255};
    glGenTextures(1,&empty_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,empty_texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,empty_image);
}

void TextureManager::Free(){
    Image* pImage;
    Texture* pTexture;
    for(byte* b : cached_textures){
        pTexture=(Texture*)b;
        if(pTexture!= null){delete pTexture;}
    }
    cached_textures.Clear();
    for(int i=0;i<texture_atlases.slots;i++){
        if(!texture_atlases.slot_is_filled.Get(i))continue;
        pImage=(Image*)texture_atlases.values[i];
        if(pImage!= null){delete pImage;}
        glDeleteTextures(1,(GLuint*)&texture_atlases.keys[i]);
    }
    texture_atlases.Clear();
}
    
Texture TextureManager::DefaultTexture(){
    return Texture();
}

Texture TextureManager::MapToAtlas(Image* texture_image){
    if(texture_image == null || texture_image->image_data==null){
        return DefaultTexture();
    }

    Texture tex;
    tex.width_px = texture_image->width;
    tex.height_px = texture_image->height;

    Image* atlas = (Image*)texture_atlases.Get(current_atlas_id);
    int map_pos[] = {0,0};
    if(IsMappable(tex.width_px,tex.height_px,map_pos)) {
        texture_image->Blit(atlas, map_pos[0], map_pos[1]);
        tex.atlas_id = current_atlas_id;
        tex.tex_coords.x = (float)map_pos[0]/ATLAS_SIZE;
        tex.tex_coords.y = (float)map_pos[1]/ATLAS_SIZE;
        tex.tex_coords.w = (float)tex.width_px/ATLAS_SIZE;
        tex.tex_coords.h = (float)tex.height_px/ATLAS_SIZE;
        SubmitImage(atlas);
    }
    //if the image is unmappable, make the texture it's own atlas.
    else if(tex.width_px >= ATLAS_SIZE || tex.height_px >= ATLAS_SIZE) {
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
        
        SubmitImage(atlas);	
    }
    return tex;
}


Texture TextureManager::Add(char* texname,Image* texture_image){
    if(texture_image == null || texture_image->image_data==null){
        logger::warn("Image for texture is empty/null: %s ",texname);
    }
    Texture texture_to_cache = MapToAtlas(texture_image);
    if(cached_textures.Get(texname)==null){
        Texture* cached_tex = (Texture*)malloc(sizeof(Texture));
        memcpy(cached_tex,&texture_to_cache,sizeof(Texture));
        cached_textures.Add(texname,(byte*)cached_tex);
    }
    return texture_to_cache;
}

Texture TextureManager::Get(char* texname){
    Texture* cache_pointer = (Texture*)cached_textures.Get(texname);
    if(cache_pointer != null) {
        return *cache_pointer;			
    }
    Image* texture_image = new Image(texname);
    return Add(texname,texture_image);
}

void TextureManager::Remove(char* texname){
    Texture* tex = (Texture*)cached_textures.Remove(texname);
    if(tex != null)delete tex;
}

