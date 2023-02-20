#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <game_project_1/io/file.hpp>
#include <game_project_1/io/serializer.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/game/entity.hpp>


#define CAMPAIGN_COUNT 1
#define CAMPAIGN_DEMO 0


class SaveEntity: public SharedEntity {
    public:
    wchar* name;
    int global_id;
    //TODO: wchar** tags for scripting lookups
    SaveEntity(int gid);

    int SavedLength();
    void Load(Deserializer& dat);
    void Save(Serializer& dat);

    void LoadFrom(ServerEntity* e);
    void CopyTo(ServerEntity* e);
};

class SavePlayer{
    public:
    int     save_id;
    int     character_global_id;
    //int   active_party_count
    //int*  active_party_units_eid
    int     last_scene;
    int     last_entrance;

    SavePlayer();
    ~SavePlayer();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

class SaveScene{
    public:
    int area_id;
    int  num_global_entities;
    int* global_entities_in_area;
    Array<SaveEntity> non_global_entities; // they're saved to the area but cannot travel outside it
    // For things like props, grounded items that we don't want to despawn forever when the scene unloads


    SaveScene();
    ~SaveScene();

    void AddGlobalEntity(int global_id);
    void RemoveGlobalID(int global_id);

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

class SaveCampaign{
    public:
    int campaign_id;
    int bitflag_len;
    byte* story_bits;
    //Todo: binary value map <String-> int,float,cstr> story_values

    SaveCampaign();
    ~SaveCampaign();

    bool GetStoryBit(int bit);
    void SetStoryBit(int bit,bool value);

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};

class SaveFile{
    private:
    static const char* savefile_extension;//=".save"
    static wchar_t* GetSaveFilePath(char* save_name);
    public:
    
    Array<SaveCampaign> campaigns;
    Pool<SaveEntity>    global_entities;
    Array<SavePlayer>   players;
    Array<SaveScene>    saved_scenes;
    
    SaveFile();
    ~SaveFile();

    void New();
    static bool Exists(char* save_name);
    void LoadOrNew(char* save_name);
    void Load(char* save_name);
    void Save(char* save_name);


    SaveCampaign*   GetCampaign(int campaign_id);
    SaveScene*      GetScene(int area_id);
    SavePlayer*     GetPlayer(int player_save_id);
    SaveEntity*     GetGlobalEntity(int global_id);

    int             GenerateSaveID();
    SavePlayer*     NewPlayer(int save_id);
    SaveScene*      StartNewSaveScene(int area_id);

    int PersistEntity(ServerEntity* e);//Registers a ServerEntity globally with a corresponding SaveEntity 
    void AssignEntityToScene(int global_id, int area_id,bool one_instance);//Saves a global entity to a scene. 
        //Can be used to pre-populate inactive scenes with existing global entities. 
        //Will scan and remove previous instance if one_instancce is true

    int SerializedLength();
};


#endif

