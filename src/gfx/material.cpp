#include "material.h"

Material::Material(){
	base_color[0]=1.0; 
	base_color[1]=1.0; 
	base_color[2]=1.0; 
	base_color[3]=1.0; 
	texture = TextureManager::DefaultTexture();
	cull_backface=false;
	metallic_factor=0;
	roughness_factor=0;
}
Material::~Material(){}
