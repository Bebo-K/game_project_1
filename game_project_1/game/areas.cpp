#include <game_project_1/game/areas.hpp>

char* Areas::error_room_map = "error_room";
Map<int,char*> Areas::area_map_names;

char* Areas::GetMapName(int area_id){
    return (area_map_names.Has(area_id))? area_map_names.Get(area_id): error_room_map;
}
void Areas::RegisterMapName(int area_id,char* map_name){
    area_map_names.Add(area_id,cstr::new_copy(map_name));
}