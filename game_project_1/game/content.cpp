#include <game_project_1/game/content.hpp>
#include <game_project_1/game/races_and_classes.hpp>
#include <game_project_1/game/items.hpp>
#include <game_project_1/game/areas.hpp>
#include <game_project_1/io/manifest.hpp>

bool GameContent::loaded=false;

void OnContentLoad(){
    if(GameContent::loaded)return;
    GameContent::loaded=true;
    
    LoadManifest<EntityRace>("dat/game/races.json","races",&Races);
    LoadManifest<EntityClass>("dat/game/classes.json","classes",&Classes);
    LoadManifest<EquipSlot>("dat/game/item_slots.json","slots",&EquipSlots);
    LoadManifest<ItemType>("dat/game/items.json","items",&ItemTypes);
    LoadManifest<Area>("dat/areas/areas.json","areas",&Areas);
    ERROR_ROOM = new Area();
    ERROR_ROOM->name= cstr::new_copy("Error room");//cleanme
    ERROR_ROOM->path= cstr::new_copy("default");//cleanme
}