#include <game_project_1/gfx/material.hpp>

Material::Material():base_color(1.0f,1.0f,1.0f,1.0f){
	texture = TextureManager::DefaultTexture();
	cull_backface=false;
	metallic_factor=0;
	roughness_factor=0;
}
Material::~Material(){}
