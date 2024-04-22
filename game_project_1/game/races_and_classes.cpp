#include <game_project_1/game/races_and_classes.hpp>

#include <game_project_1/types/str.hpp>
#include <game_project_1/io/json.hpp>
#include <game_project_1/io/manifest.hpp>

Map<int,EntityClass*> Classes(4);
Map<int,EntityRace*> Races(4);

EntityRace::EntityRace(){name=nullptr;}
EntityRace::~EntityRace(){
    DEALLOCATE(name)
}
void EntityRace::FromJson(JSONObject* json){
    name = wstr::from_cstr(json->GetString("name")->string);
    JSONArray* modelArray = json->GetArray("models");
    models.Init(modelArray->count);
    for(int i=0;i<models.length;i++){
        models[i]->FromJson(modelArray->At(i)->ObjectValue());
    }
    hitsphere_height = json->GetFloat("hitsphere_height");
    hitsphere_radius = json->GetFloat("hitsphere_radius");
    stat_base.FromJson(json->GetJObject("stat_base"));
}

EntityRaceModel::EntityRaceModel(){model_name=nullptr;}
EntityRaceModel::~EntityRaceModel(){
    DEALLOCATE(model_name)
}
void EntityRaceModel::FromJson(JSONObject* json){
    model_name = cstr::new_copy(json->GetString("id")->string);
    JSONArray* race_styles = json->GetArray("styles");
    styles.Init(race_styles->count);
    for(int i=0;i<styles.length;i++){
        styles[i]->FromJson(race_styles->At(i)->ObjectValue());
    }
    JSONArray* race_colors = json->GetArray("colorable");
    colorable.Init(race_colors->count);
    for(int i=0;i<colorable.length;i++){
        colorable[i]->FromJson(race_colors->At(i)->ObjectValue());
    }
}

EntityRaceStyle::EntityRaceStyle(){name=nullptr;model_target=nullptr;}
EntityRaceStyle::~EntityRaceStyle(){
    DEALLOCATE(name)
    DEALLOCATE(model_target)
}
void EntityRaceStyle::FromJson(JSONObject* json){
    name = wstr::from_cstr(json->GetString("name")->string);
    model_target = cstr::new_copy(json->GetString("model_target")->string);
    style_count = json->GetInt("style_count");
}

EntityRaceColor::EntityRaceColor(){
    target_count=0;
    model_targets=nullptr;
}
EntityRaceColor::~EntityRaceColor(){
    for(int i=0;i<target_count;i++){
        free(model_targets[i]);
    }
    delete[] model_targets;
    model_targets=nullptr;
    target_count=0;
}

void EntityRaceColor::FromJson(JSONObject* json){
    JSONArray* groups_array = json->GetArray("groups");
    target_count = groups_array->count;
    model_targets = new char*[target_count];
    for(int i=0;i<target_count;i++){
        model_targets[i]= cstr::new_copy(groups_array->At(i)->StringValue()->string);
    }
}

EntityClass::EntityClass(){name=nullptr;}
EntityClass::~EntityClass(){
    DEALLOCATE(name)
}
void EntityClass::FromJson(JSONObject* json){
    name = wstr::from_cstr(json->GetString("name")->string);
    stat_bonus.FromJson(json->GetJObject("stat_bonus"));
}
