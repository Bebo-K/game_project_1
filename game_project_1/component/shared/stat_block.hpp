#ifndef STAT_BLOCK_H
#define STAT_BLOCK_H

#include <game_project_1/game/stats.hpp>
#include <game_project_1/component/component.hpp>

//RPG stat block
class StatBlock: public Component{
	public:

    int level;
    StatLayer base_stats;

    StatBlock();
    StatBlock(int start_level);
    ~StatBlock();

    Component* Clone();
    virtual void Clear();

    int SerializedLength();
    void Read(Deserializer& dat);
    void Write(Serializer& dat);
};


#endif