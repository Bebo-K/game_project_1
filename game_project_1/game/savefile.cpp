#include <game_project_1/game/savefile.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/game/areas.hpp>
#include <game_project_1/game/entity_serializer.hpp>


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

SaveEntity::SaveEntity(int gid):SharedEntity(-1){global_id = gid;}
int SaveEntity::SavedLength(){
    int len=sizeof(int)*3;
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(HasComponent(i)) len += sizeof(int)+ComponentSize(i);
    }
    return len;
}
void SaveEntity::Save(Serializer& dat){
    int mask = 0;
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(HasComponent(i)) mask |= (1 << i);
    }

    dat.PutInt(id);
    dat.PutInt(global_id);
    dat.PutInt(mask);
    for(int i=0;i<COMPONENT_COUNT;i++){
        if(HasComponent(i)){
            WriteComponent(i,dat);
        }
    }
}
void SaveEntity::Load(Deserializer& dat){
    id = dat.GetInt();//may no longer exist
    global_id = dat.GetInt();
    int mask = dat.GetInt();
    for(int i=0;i<COMPONENT_COUNT;i++){
        if( (mask & (1 << i)) > 0){
            ReadComponent(i,dat);
        }
    }
}
void SaveEntity::LoadFrom(ServerEntity* e){
    Duplicate(e);
    id=e->id;
    if(e->persist != null){
        global_id = e->persist->global_id;
    }
    else{global_id=0;}
}
void SaveEntity::CopyTo(ServerEntity* e){
    int eid = e->id;
    e->Duplicate(this);
    e->id = eid;
    if(global_id != 0){
        e->persist = new Persistance();
        e->persist->global_id = global_id;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE PLAYER                                     /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

SavePlayer::SavePlayer(){
    save_id=0;
    character_global_id=0;
    last_scene = 0;
    last_entrance = 0;
}

SavePlayer::~SavePlayer(){
    save_id=0;
    character_global_id=0;
    last_scene = 0;
    last_entrance = 0;
}
int SavePlayer::SerializedLength(){
    return sizeof(int)*4;
}
void SavePlayer::Read(Deserializer& dat){
    save_id = dat.GetInt();
    character_global_id = dat.GetInt();
    last_scene = dat.GetInt();
    last_entrance = dat.GetInt();
}
void SavePlayer::Write(Serializer& dat){
    dat.PutInt(save_id);
    dat.PutInt(character_global_id);
    dat.PutInt(last_scene);
    dat.PutInt(last_entrance);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE SCENE                                      /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

SaveScene::SaveScene():global_entities_in_area(),non_global_entities(){
    area_id=0;
}
SaveScene::~SaveScene(){
    if(global_entities_in_area != nullptr)delete[] global_entities_in_area;
    non_global_entities.Destroy();
}

void SaveScene::AddGlobalEntity(int global_id){
    for(int i=0;i<num_global_entities;i++){
        if(global_entities_in_area[i] == global_id)return;
    }

    num_global_entities++;
    int* new_global_entities_in_area = new int[num_global_entities];
    new_global_entities_in_area[num_global_entities-1]=global_id;

    delete[] global_entities_in_area;
    global_entities_in_area = new_global_entities_in_area;
}
void SaveScene::RemoveGlobalID(int global_id){
    for(int i=0;i<num_global_entities;i++){
        if(global_entities_in_area[i] == global_id){
            if(num_global_entities == 1){
                num_global_entities=0;
                delete[] global_entities_in_area;
                global_entities_in_area = nullptr;
                return;
            }

            int* new_global_entities_in_area = new int[num_global_entities-1];
            for(int j=0;j<i;j++){
                new_global_entities_in_area[j]=global_entities_in_area[j];
            }
            for(int j=i+1;j<num_global_entities;j++){
                new_global_entities_in_area[j-1]=global_entities_in_area[j];
            }
            new_global_entities_in_area[num_global_entities-1]=global_id;

            delete[] global_entities_in_area;
            global_entities_in_area = new_global_entities_in_area;
            num_global_entities--;
        }
    }
}

int SaveScene::SerializedLength(){
    int size=sizeof(int);//area_id
    size+=sizeof(int);//num_global_entities
    size+=num_global_entities*sizeof(int);//global_entities_in_area
    size+=sizeof(int);//non_global_entities.length
    for(SaveEntity* entity:non_global_entities){
        size += entity->SavedLength();//non_global_entities
    }
    return size;
}
void SaveScene::Read(Deserializer& dat){
    area_id = dat.GetInt();
    num_global_entities = dat.GetInt();
    global_entities_in_area = (num_global_entities>0)?new int[num_global_entities] : nullptr;
    for(int i=0;i<num_global_entities;i++){
        global_entities_in_area[i] = dat.GetInt();
    }   
    non_global_entities.Resize(dat.GetInt());
    for(SaveEntity* entity:non_global_entities){
        entity->Load(dat);
    }
}
    
void SaveScene::Write(Serializer& dat){
    dat.PutInt(area_id);
    dat.PutInt(num_global_entities);
    for(int i=0; i<num_global_entities;i++){
        dat.PutInt(global_entities_in_area[i]);
    }
    dat.PutInt(non_global_entities.length);
    for(SaveEntity* entity:non_global_entities){
        entity->Save(dat);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE CAMPAIGN                                   /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveCampaign::SaveCampaign(){
    campaign_id=0;
    bitflag_len=0;
    story_bits=nullptr;
}
SaveCampaign::~SaveCampaign(){
    campaign_id=0;
    bitflag_len=0;
    if(story_bits != nullptr){free(story_bits);story_bits=nullptr;}
}
void SaveCampaign::Read(Deserializer& dat){
    campaign_id = dat.GetInt();
    bitflag_len = dat.GetInt();
    int bitflag_size_bytes = (bitflag_len/8);
    if(bitflag_len%8 > 0){bitflag_size_bytes++;}
    story_bits = dat.CopyBytes(bitflag_size_bytes);
}
void SaveCampaign::Write(Serializer& dat){
    dat.PutInt(campaign_id);
    dat.PutInt(bitflag_len);
    int bitflat_size_bytes = (bitflag_len/8);
    if(bitflag_len%8 > 0){bitflat_size_bytes++;}
    dat.WriteBytes(story_bits,bitflat_size_bytes);
}
int SaveCampaign::SerializedLength(){
    int size=0;
    size += sizeof(int);
    size += sizeof(int);
    size += (bitflag_len/8); if(bitflag_len%8 > 0){size++;}
    return size;
}
bool SaveCampaign::GetStoryBit(int bit){
    if(bit < 0 || bit > bitflag_len){
        logger::exception("Attempting to get invalid story bit %d in campaign id %d",bit,campaign_id);
    }
    return ((story_bits[bit/8] & (1 << bit%8)) != 0);
}
void SaveCampaign::SetStoryBit(int bit,bool value){
    if(bit < 0 || bit > bitflag_len){
        logger::exception("Attempting to set invalid story bit %d in campaign id %d",bit,campaign_id);
    }
    if(value){story_bits[bit/8] |= (1<<(bit%8));}
    else{story_bits[bit/8] &= ~(1<<(bit%8));}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////                               SAVE FILE                                       /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
SaveFile::SaveFile():campaigns(),global_entities(),players(),saved_scenes(){}
SaveFile::~SaveFile(){
    players.Destroy();
    campaigns.Destroy();
    global_entities.Clear();
    saved_scenes.Destroy();
}

void SaveFile::New(){
    campaigns.Allocate(CAMPAIGN_COUNT);
    
    //Demo campaign
    campaigns[0]->campaign_id=1;
    campaigns[0]->bitflag_len=32;
    campaigns[0]->story_bits=(byte*)calloc(4,sizeof(byte));

}
bool SaveFile::Exists(char* save_name){return UserFile::Exists(GetSaveFilePath(save_name));}
void SaveFile::LoadOrNew(char* save_name){
    if(false/*(TODO: toggled off for dev work)SaveFile::Exists(save_name)*/){
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
   
    Deserializer data(buffer);

    campaigns.Allocate(data.GetInt());
    for(SaveCampaign* campaign:campaigns){
        campaign->Read(data);
    }
    int global_entity_count = data.GetInt();
    for(int i=0;i<global_entity_count;i++){
        SaveEntity* new_global_entity = new (global_entities.Allocate()) SaveEntity(0);
        new_global_entity->Load(data);
    }
    players.Allocate(data.GetInt());
    for(SavePlayer* player:players){
        player->Read(data);
    }
    saved_scenes.Allocate(data.GetInt());
    for(SaveScene* scene:saved_scenes){
        scene->Read(data);
    }

    free(buffer);
    save_file.close();
}
void SaveFile::Save(char* save_name){
    UserFile save_file(GetSaveFilePath(save_name),'w');

    int serialized_length = SerializedLength();
    byte* buffer = (byte*)calloc(serialized_length,1);
    Serializer data(buffer,serialized_length);

    data.PutInt(campaigns.length);
    for(SaveCampaign* campaign:campaigns){
        campaign->Write(data);
    }
    data.PutInt(global_entities.Count());
    for(SaveEntity* entity:global_entities){
        entity->Save(data);
    }
    data.PutInt(players.length);
    for(SavePlayer* player:players){
        player->Write(data);
    }
    data.PutInt(saved_scenes.length);
    for(SaveScene* scene:saved_scenes){
        scene->Write(data);
    }

    save_file.write(buffer,serialized_length);
    free(buffer);
    save_file.close();
    logger::info("Savefile '%s' saved.\n",save_name);
}
int SaveFile::SerializedLength(){
    int size=sizeof(int);
    for(SaveCampaign* campaign:campaigns){size+= campaign->SerializedLength();}
    size+=sizeof(int);
    for(SaveEntity* entity:global_entities){size += entity->SavedLength();}
    size+=sizeof(int);
    for(SavePlayer* player:players){size += player->SerializedLength();}
    size+=sizeof(int);
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

int SaveFile::GenerateSaveID(){
    int ret;
    do{ret= abs(rand());}while(ret == 0||GetPlayer(ret) != nullptr);
    return ret;    
}

SavePlayer* SaveFile::NewPlayer(int save_id){   
    if(save_id == 0){
        logger::warn("Cannot start a new save ID 0: invalid id");
        return nullptr;
    }
    if(GetPlayer(save_id) != nullptr){
        logger::warn("Cannot start a new save ID %d: save exists.",save_id);
        return nullptr;
    }
    players.Allocate(players.length+1);
    SavePlayer* new_player = players[players.length-1];
        new_player->save_id = save_id;
        new_player->character_global_id = 0;
        new_player->last_scene =  Areas::save_start_id;
        new_player->last_entrance =  Areas::save_start_entrance;
    return new_player;
}

int SaveFile::PersistEntity(ServerEntity* e){
    int new_global_id =0;
    do{new_global_id = abs(rand());} while(new_global_id ==0 || GetGlobalEntity(new_global_id)!= nullptr);
    SaveEntity* new_global_entity = new (global_entities.Allocate()) SaveEntity(new_global_id);
    new_global_entity->LoadFrom(e);
    new_global_entity->global_id = new_global_id;
    e->persist = new Persistance();
    e->persist->global_id = new_global_id;
    return new_global_id;
}

void SaveFile::AssignEntityToScene(int global_id, int area_id,bool one_instance){
    for(SaveScene* scene:saved_scenes){
        if(one_instance){scene->RemoveGlobalID(global_id);}
        if(area_id == scene->area_id){scene->AddGlobalEntity(global_id);}
    }
}