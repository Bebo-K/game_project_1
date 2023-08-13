#include <game_project_1/game/areas.hpp>

#include <game_project_1/content/base_content.hpp>
#include <game_project_1/core/entity.hpp>

Map<int,char*> Areas::map_names;


bool Areas::ValidArea(int area_id){
    if(area_id == Areas::error_room){return true;}
    return map_names.Has(area_id);
}
char* Areas::GetMap(int area_id){
    if(area_id == Areas::error_room){return "error_room";}
    return map_names.Get(area_id);
}
void Areas::RegisterMap(int area_id,char* map_name){
    map_names.Add(area_id,cstr::new_copy(map_name));
}

