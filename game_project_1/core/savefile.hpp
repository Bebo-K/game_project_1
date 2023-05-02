#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <game_project_1/io/file.hpp>
#include <game_project_1/io/serializer.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/types/list.hpp>
#include <game_project_1/core/entity.hpp>


#define CAMPAIGN_COUNT 1
#define CAMPAIGN_DEMO 0


class SaveEntity: ServerEntity{
    public:
    int global_id;
    //TODO: wchar** tags for scripting lookups

    SaveEntity(int gid);
    SaveEntity(int gid, ServerEntity* e);
    ~SaveEntity();
    void SetFromEntity(ServerEntity* e);
    void Instantiate(ServerEntity* e);

    void Save(Serializer& dat);
    void Load(Deserializer& dat);
    int SavedLength();
};

class SavePlayer{
    public:
    int             save_id;//player-unique save id- does not correlate with player slot in server
    SaveEntity      character;
    //int           party_count
    //SaveEntity*   party_units
    int             last_scene;
    int             last_entrance;

    SavePlayer(int sid);
    ~SavePlayer();

    void Save(Serializer& dat);
    void Load(Deserializer& dat);
    int SavedLength();
};

class SaveScene{
    public:
    int area_id;
    IntegerSet global_entities;
    Array<SaveEntity> local_entities; // they're saved to the area but cannot travel outside it
    // For things like props, grounded items that we don't want to despawn forever when the scene unloads

    SaveScene(int area);
    ~SaveScene();

    void Save(Serializer& dat);
    void Load(Deserializer& dat);
    int SavedLength();
};

class SaveCampaign{
    public:
    int campaign_id;
    BitArray story_bits;
    //Todo: binary value map <String-> int,float,cstr> story_values

    SaveCampaign(int cid);
    ~SaveCampaign();

    void Save(Serializer& dat);
    void Load(Deserializer& dat);
    int SavedLength();
};

class SaveFile{
    private:
    static const char* savefile_extension;//=".save"
    static wchar_t* GetSaveFilePath(char* save_name);
    public:
    
    List<SaveCampaign> campaigns;
    List<SaveEntity>   global_entities;//Entities that persist scene boundaries.
    List<SavePlayer>   players;
    List<SaveScene>    saved_scenes;
    
    SaveFile();
    ~SaveFile();

    void New();
    void LoadOrNew(char* save_name);
    void Load(char* save_name);
    void Save(char* save_name);
    int SavedLength();


    SaveCampaign*   GetCampaign(int campaign_id);
    SaveScene*      GetScene(int area_id);
    SavePlayer*     GetPlayer(int player_number);
    ServerEntity*   GetGlobalEntity(int global_id);

    //
    //int           GenerateGlobalEntityID();
    SavePlayer*     NewPlayer();
    SaveScene*      NewScene(int area_id);
    SaveCampaign*   NewCampaign(int campaign_id);
    void            NewGlobalEntity(ServerEntity* e);

    SaveScene*      GetOrNewScene(int area_id);

};


#endif

