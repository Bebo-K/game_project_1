#include <game_project_1/game/areas.hpp>

Map<int,char*> Areas::map_names;


bool Areas::ValidArea(int area_id){
    if(area_id == Areas::error_room){return true;}
    return area_map_names.Has(area_id);
}
char* Areas::GetMap(int area_id){
    if(area_id == Areas::error_room){return "error_room";}
    return area_map_names.Get(area_id);
}
void Areas::RegisterMap(int area_id,char* map_name){
    area_map_names.Add(area_id,cstr::new_copy(map_name));
}

void LoadTestArea(){
    ServerEntity* friendly = CreateEntity(SpawnType::APPEAR);
        Identity* friendly_id = friendly->Get<Identity>();
            friendly_id->name = wstr::new_copy(L"friendly");
            friendly_id->type = BaseContent::HUMANOID;
        Character* friendly_char = new Character();
            friendly_char->race_id=Races::Human.id;
            friendly_char->class_id=Classes::Archer.id;
        friendly->Add<Character>(friendly_char);
        friendly->ServerAdd<NPCState>(new NPCState(BaseContent::NPC_WANDER));
    
    Location place;
    place.position = {7,4,1};
    place.rotation.y = 183;
    BuildEntity(friendly,place);
}