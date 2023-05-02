#include <game_project_1/core/savefile.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/game/areas.hpp>
#include <game_project_1/component/server/persistence.hpp>


const char* SaveFile::savefile_extension = ".save";

wchar_t* SaveFile::GetSaveFilePath(char* save_name){
    int save_directory_len = wcslen(config::save_directory);
    int save_name_len = cstr::len(save_name);
    int save_extension_len = cstr::len(savefile_extension);
    wchar_t* save_file_path = (wchar_t*)calloc(save_directory_len+1+save_name_len+save_extension_len+1,sizeof(wchar_t));

    memcpy(save_file_path,config::save_directory,save_directory_len*sizeof(wchar_t));
    save_file_path[save_directory_len]=OS_PATH_SEPERATOR;
    for(int i=0;i<save_name_len;i++){
        save_file_path[save_directory_len+1+i] = (wchar_t)save_name[i];
    }
    for(int i=0;i<save_extension_len;i++){
        save_file_path[save_directory_len+1+save_name_len+i] = (wchar_t)savefile_extension[i];
    }
    return save_file_path;
}






/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE ENTITY                                     /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveEntity::SaveEntity(int gid):ServerEntity(-1){global_id = gid;}
SaveEntity::SaveEntity(int gid, ServerEntity* e){
    global_id=gid;
    e->CloneTo(this);
}
SaveEntity::~SaveEntity(){ServerEntity::Clear();}
void SaveEntity::SetFromEntity(ServerEntity* e){
    Clear();
    e->CloneTo(this);
}
void SaveEntity::Instantiate(ServerEntity* e){
    ServerEntity::CloneTo(e);
}
void SaveEntity::Save(Serializer& dat){
    dat.PutInt(global_id);
    Write(dat,bitmask::all);
}
void SaveEntity::Load(Deserializer& dat){
    global_id = dat.GetInt();//may no longer exist
    Read(dat,OS::time_ms());
}
int SaveEntity::SavedLength(){
    return sizeof(int) + SerializedLength(bitmask::all);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE PLAYER                                     /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SavePlayer::SavePlayer(int id):character(0){
    save_id=id;
    last_scene = 0;
    last_entrance = 0;
}
SavePlayer::~SavePlayer(){}

void SavePlayer::Save(Serializer& dat){
    dat.PutInt(save_id);
    dat.PutInt(last_scene);
    dat.PutInt(last_entrance);
    character.Save(dat);
}

void SavePlayer::Load(Deserializer& dat){
    save_id = dat.GetInt();
    last_scene = dat.GetInt();
    last_entrance = dat.GetInt();
    character.Read(dat);
}

int SavePlayer::SavedLength(){
    return sizeof(int)*3+character.SavedLength();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE SCENE                                      /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveScene::SaveScene(int area):global_entities(),local_entities(){
    area_id=area;
}
SaveScene::~SaveScene(){
    global_entities.Clear();
    local_entities.Destroy();
}

void SaveScene::Save(Serializer& dat){
    dat.PutInt(area_id);
    dat.PutInt(global_entities.length);
    for(int i=0; i<global_entities.length;i++){
        dat.PutInt(global_entities_in_area[i]);
    }
    dat.PutInt(local_entities.length);
    for(SaveEntity* entity:local_entities){
        entity->Save(dat);
    }
}

void SaveScene::Load(Deserializer& dat){
    area_id = dat.GetInt();
    int num_global_entities = dat.GetInt();
    for(int i=0;i<num_global_entities;i++){
        global_entities.Add(dat.GetInt());
    }   
    local_entities.Resize(dat.GetInt());
    for(SaveEntity* entity:local_entities){
        entity->Load(dat);
    }
}

int SaveScene::SavedLength(){
    int size=sizeof(int);//area_id
    size+=sizeof(int) * (1 + global_entities.length); //global_entities
    size+=sizeof(int);//local_entities.length
    for(SaveEntity* entity:local_entities){
        size += entity->SavedLength();//local_entities
    }
    return size;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE CAMPAIGN                                   /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveCampaign::SaveCampaign(int cid):story_bits(){
    campaign_id=cid;
}
SaveCampaign::~SaveCampaign(){}

void SaveCampaign::Save(Serializer& dat){
    dat.PutInt(campaign_id);
    dat.PutInt(story_bits.bits);
    int bitflag_size_bytes = (story_bits.bits/8);
    bitflag_size_bytes += (bitflag_size_bytes%8 > 0)1:0;
    dat.WriteBytes(story_bits.data,bitflag_size_bytes);
}

void SaveCampaign::Load(Deserializer& dat){
    campaign_id = dat.GetInt();
    story_bits.Initialize(dat.GetInt());
    int bitflag_size_bytes = (story_bits.bits/8);
    bitflag_size_bytes += (bitflag_size_bytes%8 > 0)1:0;
    dat.CopyBytesTo(story_bits.data,bitflag_size_bytes);
}

int SaveCampaign::SavedLength(){
    int bitflag_size_bytes = (story_bits.bits/8);
    bitflag_size_bytes += (bitflag_size_bytes%8 > 0)1:0;

    return sizeof(int)*2 + bitflag_size_bytes;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE FILE                                       /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveFile::SaveFile():campaigns(),global_entities(),players(),saved_scenes(){}
SaveFile::~SaveFile(){
    for(SaveCampaign* campaign:campaigns){campaign->~SaveCampaign();}
    campaigns.Clear();
    for(SaveEntity* entity: global_entities){entity->~SaveEntity();}
    global_entities.Clear();
    for(SavePlayer* player: players){player->~SavePlayer();}
    players.Clear();
    for(SaveScene* scene: saved_scenes){scene->~SaveScene();}
    saved_scenes.Clear();
}

void SaveFile::New(){
    campaigns.Add(new SaveCampaign(1));
}

void SaveFile::LoadOrNew(char* save_name){
    if(false/*(TODO: toggled off for dev work)UserFile::Exists(GetSaveFilePath(save_name));*/){
        logger::info("Loading savefile '%s'...\n",save_name);
        Load(server_config::save_name);
    }
    else{
        logger::info("No save file found for save '%s', starting a new one.\n",save_name);
        New();
    }
    logger::info("done\n");
}

void SaveFile::Load(char* save_name){
    UserFile save_file(GetSaveFilePath(save_name),'r');
    if(save_file.error || save_file.length <= 0){
        logger::warn("Savefile %s cannot be read.",save_name);
        return;
    }

    byte* buffer = (byte*)calloc(save_file.length,1);
    save_file.read(buffer,save_file.length);
    save_file.close();
    Deserializer data(buffer);

    int campaign_count = data.GetInt();
    for(int i=0;i<campaign_count;i++){
        SaveCampaign* campaign = new Campaign(0);
        campaign->Load(data);
        campaigns.Add(campaign);
    }
    int entity_count = data.GetInt();
    for(int i=0;i<entity_count;i++){
        SaveEntity* entity = new SaveEntity(0);
        entity->Load(data);
        global_entities.Add(entity);
    }
    int player_count = data.GetInt();
    for(int i=0;i<player_count;i++){
        SavePlayer* player = new SavePlayer(0);
        player->Load(data);
        players.Add(player);
    }
    int scene_count = data.GetInt();
    for(int i=0;i<scene_count;i++){
        SaveScene* scene = new SaveScene(0);
        scene->Load(data);
        saved_scenes.Add(scene);
    }
    free(buffer);
}

void SaveFile::Save(char* save_name){
    int serialized_length = SerializedLength();
    byte* buffer = (byte*)calloc(serialized_length,1);
    Serializer data(buffer,serialized_length);

    data.PutInt(campaigns.Count());
    for(SaveCampaign* campaign:campaigns){
        campaign->Write(data);
    }
    data.PutInt(global_entities.Count());
    for(SaveEntity* entity:global_entities){
        entity->Save(data);
    }
    data.PutInt(players.Count());
    for(SavePlayer* player:players){
        player->Write(data);
    }
    data.PutInt(saved_scenes.Count());
    for(SaveScene* scene:saved_scenes){
        scene->Write(data);
    }

    UserFile save_file(GetSaveFilePath(save_name),'w');
    save_file.write(buffer,serialized_length);
    free(buffer);
    save_file.close();
    logger::info("Savefile '%s' saved.\n",save_name);
}

int SaveFile::SavedLength(){
    int size=sizeof(int);//campaigns.length
    for(SaveCampaign* campaigns:campaigns){size+= campaign->SerializedLength();}
    size+=sizeof(int);//global_entities.length
    for(SaveEntity* entity:global_entities){size += entity->SavedLength();}
    size+=sizeof(int);//players.length
    for(SavePlayer* player:players){size += player->SerializedLength();}
    size+=sizeof(int);//saved_scenes.length
    for(SaveScene* scene:saved_scenes){size += scene->SerializedLength();}
    return size;
}

SaveCampaign* SaveFile::GetCampaign(int campaign_id){
    for(SaveCampaign* campaign:campaigns){if(campaign->campaign_id==campaign_id)return campaign;}
    return nullptr;
}

SaveScene* SaveFile::GetScene(int area_id){
    for(SaveScene* scene:saved_scenes){if(scene->area_id == area_id)return scene;}
    return nullptr;
}

SavePlayer* SaveFile::GetPlayer(int save_id){
    for(SavePlayer* player:players){if(player->save_id==save_id)return player;}
    return nullptr;
}

SaveEntity* SaveFile::GetGlobalEntity(int global_id){
    for(SaveEntity* save_entity:global_entities){
        if(save_entity->global_id == global_id)return save_entity;
    }
    return nullptr;
}

SavePlayer* SaveFile::NewPlayer(){   
    int save_id;
    do{save_id= abs(rand());}while(save_id == 0||GetPlayer(save_id) != nullptr);

    SavePlayer* new_player = new SavePlayer(save_id);
        new_player->last_scene =  Areas::save_start_id;
        new_player->last_entrance =  Areas::save_start_entrance;
    players.Add(new_player);
    return new_player;
}

SaveScene* SaveFile::NewScene(int area_id){
    if(GetScene(area_id)){
        logger::exception("SaveFile::NewScene() -> Scene ID %s already has save entry.\n",area_id);
    }
    SaveScene* new_scene = new SaveScene(area_id);
    saved_scenes.Add(new_scene);
    return new_scene;
}

SaveCampaign* SaveFile::NewCampaign(int campaign_id){
    if(GetCampaign(campaign_id)){
        logger::exception("SaveFile::NewCampaign() -> Campaign ID %s already has save entry.\n",campaign_id);
    }
    SaveCampaign* new_campaign = new SaveCampaign(campaign_id);
    campaigns.Add(new_campaign);
    return new_campaign;
}

int SaveFile::NewGlobalEntity(ServerEntity* e){
    int new_global_id =0;
    do{new_global_id = abs(rand());} while(new_global_id ==0 || GetGlobalEntity(new_global_id)!= nullptr);

    SaveEntity* global_entity = new SaveEntity(new_global_id,e);
    global_entities.Add(global_entity);
    e->ServerAdd<Persistence>(new Persistence(global_entity));
    return new_global_id;
}

SaveScene* SaveFile::GetOrNewScene(int area_id){
    SaveScene* ret = GetScene(area_id);
    if(ret != null)return ret;
    ret = new SaveScene(area_id);
    saved_scenes.Add(ret);
    return ret;
}

void SaveFile::AssignEntityToScene(int global_id, int area_id,bool one_instance){
    for(SaveScene* scene:saved_scenes){
        if(one_instance){scene->RemoveGlobalID(global_id);}
        if(area_id == scene->area_id){scene->AddGlobalEntity(global_id);}
    }
}