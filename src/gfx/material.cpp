#include "material.h"


Material::Material(){
	name="Default";
	texture = TextureManager::DefaultTexture();
	cull_backface=false;
	metallic_factor=0;
	roughness_factor=0;
}
Material::Material(const char* texture_filename){
	name="Default";
	texture = TextureManager::Get(texture_filename);
	cull_backface=false;
	metallic_factor=0;
	roughness_factor=0;
}

