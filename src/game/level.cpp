#include "level.h"


Level::Level(){
    skybox = nullptr;
    geometry = nullptr;

}

void Level::AddToRenderer(Renderer* r){
    if(skybox != nullptr){r->Add(skybox);}
    if(geometry != nullptr){r->Add(geometry);}
}
void Level::RemoveFromRenderer(Renderer* r){
    if(skybox != nullptr){r->Remove(skybox);}
    if(geometry != nullptr){r->Remove(geometry);}
}

void Level::LoadDefault(){

}

void Level::LoadFromJSON(JSONObject json){






}

void Level::Unload(){
    


}