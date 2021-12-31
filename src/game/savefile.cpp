#include "savefile.h"
#include "../struct/str.h"
#include "../log.h"
#include "../config.h"


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


SavePlayer::SavePlayer(){
    player_id=0;
    player_name=nullptr;
    player_scene=0;
    player_scene_entrance=0;
}

SavePlayer::~SavePlayer(){
    player_id=0;
    player_scene=0;
    player_scene_entrance=0;
    if(player_name != nullptr){free(player_name);player_name=nullptr;}
}

void SavePlayer::Read(Deserializer& dat){
    player_id = dat.GetInt();
    player_name = dat.GetString();
    player_scene = dat.GetInt();
    player_scene_entrance = dat.GetInt();
}
void SavePlayer::Write(Serializer& dat){
    dat.PutInt(player_id);
    dat.PutString(player_name);
    dat.PutInt(player_scene);
    dat.PutInt(player_scene_entrance);
}
int SavePlayer::SerializedLength(){
    int size = 0;
    size += sizeof(int);
    size += cstr::len(player_name)+1;
    size += sizeof(int);
    size += sizeof(int);

    return size;
}


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



void BuildDemoCampaign(SaveCampaign* campaign){
    campaign->campaign_id=-1;
    campaign->bitflag_len=32;
    campaign->story_bits=(byte*)calloc(4,sizeof(byte));
}

void SpawnPlayerInitialDemo(SavePlayer* player){
    player->player_scene = 0;
    player->player_scene_entrance = 0;
}

SaveFile::SaveFile(){
    saved_campaigns=0;
    saved_players=0;
    players=nullptr;
    campaigns=nullptr;
}
SaveFile::~SaveFile(){
    if(players != nullptr){
        for(int i=0;i<saved_players;i++){players[i].~SavePlayer();}
        free(players);
        players=nullptr;}
    if(campaigns != nullptr){
        for(int i=0;i<saved_campaigns;i++){campaigns[i].~SaveCampaign();}
        free(campaigns);
        campaigns=nullptr;}
    saved_campaigns=0;
    saved_players=0;
}
void SaveFile::New(){
    saved_campaigns = CAMPAIGN_COUNT;
    campaigns = (SaveCampaign*)calloc(CAMPAIGN_COUNT,sizeof(SaveCampaign));
    BuildDemoCampaign(&campaigns[0]);
}
bool SaveFile::Exists(char* save_name){
    return UserFile::Exists(GetSaveFilePath(save_name));
}
void SaveFile::Load(char* save_name){
    UserFile save_file(GetSaveFilePath(save_name),'r');

    byte* data = (byte*)calloc(save_file.length,1);
    save_file.read(data,save_file.length);
    Deserialize(data);

    free(data);
    save_file.close();
}
void SaveFile::Save(char* save_name){
    UserFile save_file(GetSaveFilePath(save_name),'w');

    byte* data = (byte*)calloc(SerializedLength(),1);
    int data_len = Serialize(data);
    save_file.write(data,data_len);

    free(data);
    save_file.close();
}
int SaveFile::Serialize(byte* data){
    Serializer dat(data,SerializedLength());

    dat.PutInt(saved_players);
    for(int i=0;i<saved_players;i++){
        players[i].Write(dat);
    }
    dat.PutInt(saved_campaigns);
    for(int i=0;i<saved_campaigns;i++){
        campaigns[i].Write(dat);
    }

    return dat.data_length;
}
void SaveFile::Deserialize(byte* src){
    Deserializer dat(src);

    saved_players = dat.GetInt();
    players = (SavePlayer*)calloc(saved_players,sizeof(SavePlayer));
    for(int i=0;i<saved_players;i++){
        players[i].Read(dat);
    }
    saved_campaigns = dat.GetInt();
    campaigns = (SaveCampaign*)calloc(saved_campaigns,sizeof(SaveCampaign));
    for(int i=0;i<saved_campaigns;i++){
        campaigns[i].Read(dat);
    }
}

int SaveFile::SerializedLength(){
    int size=0;
    size+=4;//saved_players
    for(int i=0;i<saved_players;i++){
        size+=players[i].SerializedLength();
    }
    size+=4;//saved_campaigns
    for(int i=0;i<saved_campaigns;i++){
       size+=campaigns[i].SerializedLength();
    }
    return size;
}

SaveCampaign* SaveFile::GetCampaign(int campaign_id){
    for(int i=0;i<saved_campaigns;i++){if(campaigns[i].campaign_id==campaign_id)return &campaigns[i];}
    return nullptr;
}
SavePlayer* SaveFile::GetPlayer(int player_id){
    for(int i=0;i<saved_campaigns;i++){if(players[i].player_id==player_id)return &players[i];}
    return nullptr;
}
SavePlayer* SaveFile::GetPlayer(char* player_name){
    for(int i=0;i<saved_campaigns;i++){
        if(cstr::compare(player_name,players[i].player_name))return &players[i];
    }
    return nullptr;
}
SavePlayer* SaveFile::NewPlayer(int player_id,char* player_name){
    byte* newPlayerSave = (byte*)calloc(saved_players+1,sizeof(SavePlayer));
    
    if(saved_players> 0){
        memcpy(newPlayerSave,players,sizeof(SavePlayer)*saved_players);
        free(players);//free old memory but don't call delete
    }
    players = (SavePlayer*)newPlayerSave;
    saved_players++;
    SavePlayer* ret = &players[saved_players-1];
    ret->player_name = cstr::new_copy(player_name);
    ret->player_id = player_id;
    SpawnPlayerInitialDemo(ret);
    return ret;
}