#include "material.h"


Material::Material(){
	texture = TextureManager::DefaultTexture();
	ambient = {0.5,0.5,0.5};
	diffuse = {1,1,1};
	specular = {0,0,0};
}
Material::Material(const char* texture_filename){
	texture = TextureManager::Get(texture_filename);
	ambient = {0.1,0.1,0.1};
	diffuse = {1,1,1};
	specular = {0,0,0};
}

