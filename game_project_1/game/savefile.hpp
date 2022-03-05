#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <game_project_1/io/file.hpp>
#include <game_project_1/io/serializer.hpp>
#include <game_project_1/game/game_constants.hpp>
#include <game_project_1/game/unit.hpp>


#define CAMPAIGN_COUNT 1

#define CAMPAIGN_DEMO 0

class SaveUnit{
    public:

    wchar* name;
    int race;
    UnitAppearance appearance;
    // UnitInventory inventory
    // UnitStats stats

    SaveUnit();
    SaveUnit(GameUnit from);
    GameUnit ToUnit();
};

class SavePlayer{
    public:
    int   save_id;
    wchar* player_name;
    int   player_scene;
    int   player_scene_entrance;
    SaveUnit player_unit;
    //int party_members;
    //SaveUnit party_units;

    SavePlayer();
    ~SavePlayer();
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
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
    int SerializedLength();

    bool GetStoryBit(int bit);
    void SetStoryBit(int bit,bool value);
};

class SaveFile{
    private:
    static const char* savefile_extension;//=".save"
    static wchar_t* GetSaveFilePath(char* save_name);
    public:
    int           saved_players;
    SavePlayer*   players;
    int           saved_campaigns;
    SaveCampaign* campaigns;
    
    SaveFile();
    ~SaveFile();
    void New();
    static bool Exists(char* save_name);
    void LoadOrNew(char* save_name);
    void Load(char* save_name);
    void Save(char* save_name);
    int Serialize(byte* bytes);
    int SerializedLength();
    void  Deserialize(byte* src);

    SaveCampaign* GetCampaign(int campaign_id);
    SavePlayer* GetPlayer(wchar* player_name);
    int GetPlayerSaveID(wchar* player_name);
    SavePlayer* GetPlayerByID(int save_id);
    SavePlayer* NewPlayer(wchar* player_name,UnitAppearance appearance);


};


#endif

