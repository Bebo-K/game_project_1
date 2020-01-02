#include "material.h"


Texture::Texture() {
	atlas_id = -1;
	tex_coords = {0.0f,0.0f,1.0f,1.0f};
	width_px=height_px=0;
}

TextureRectangle Texture::GetSubTexture(int x,int y,int w,int h) {
	TextureRectangle ret = {0,0,0,0};
	
	ret.x = tex_coords.x + (tex_coords.w/width_px) * x;
	ret.y = tex_coords.y + (tex_coords.h/height_px) * h;
	ret.w = (tex_coords.w/width_px) * h;
	ret.h = (tex_coords.h/height_px) * w;
	return ret;
}

Material::Material(){
	texture = Texture();

}

Material::Material(char* texture_filename){
	texture = Texture();
	//TODO: this

}