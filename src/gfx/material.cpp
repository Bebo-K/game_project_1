#include "material.h"



Material::Material(char* texture_filename){
	texture = TextureManager::Get(texture_filename);
	ambient = {0,0,0};
	diffuse = {1,1,1};
	specular = {0,0,0};
	emissive = {0,0,0};
}

