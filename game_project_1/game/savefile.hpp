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
    int race_id;
    int class_id;
    UnitAppearance appearance;
    // UnitInventory inventory
    // UnitStats stats

    SaveUnit();
    SaveUnit(GameUnit from);
    GameUnit* ToUnit();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
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

    SaveCampaign* GetCampaign(int campaign_id);
    SavePlayer* GetPlayer(wchar* player_name);
    int GetPlayerSaveID(wchar* player_name);
    SavePlayer* GetPlayerByID(int save_id);
    SavePlayer* NewPlayer(wchar* player_name,int race_id,int class_id, UnitAppearance appearance);

    int SerializedLength();
    void  Deserialize(byte* src);
    int Serialize(byte* bytes);
};


#endif

