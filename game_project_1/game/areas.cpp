#include <game_project_1/game/areas.hpp>

#include <game_project_1/content/base_content.hpp>
#include <game_project_1/core/entity.hpp>

Map<int,Area*> Areas(4);
Area* ERROR_ROOM=nullptr;


Area::Area(){
    name=nullptr;
    path=nullptr;
}
Area::~Area(){
    if(name != nullptr)free(name);
    name=nullptr;
    if(path != nullptr)free(path);
    path=nullptr;
}
void Area::FromJson(JSONObject* json){
    this->name = cstr::new_copy(json->GetString("name")->string);
    this->path = cstr::new_copy(json->GetString("path")->string);
}

