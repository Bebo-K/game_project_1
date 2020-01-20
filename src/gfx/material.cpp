#include "material.h"



Material::Material(const char* texture_filename){
	texture = TextureManager::Get(texture_filename);
	ambient = {0.1,0.1,0.1};
	diffuse = {1,1,1};
	specular = {0,0,0};
}

