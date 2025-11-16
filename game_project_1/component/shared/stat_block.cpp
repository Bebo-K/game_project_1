#include <game_project_1/component/shared/stat_block.hpp>


StatBlock::StatBlock(){Clear();level=1;}

StatBlock::StatBlock(int start_level){Clear();level=start_level;}

StatBlock::~StatBlock(){}

int StatBlock::SerializedLength(){return sizeof(int)+base_stats.SerializedLength();}

void StatBlock::Read(Deserializer& dat){
    level=dat.GetInt();
    base_stats.Read(dat);
}

void StatBlock::Write(Serializer& dat){
    dat.PutInt(level);
    base_stats.Write(dat);
}

Component* StatBlock::Clone(ComponentParentContext context){
    StatBlock* copy = new StatBlock(level);
    copy->base_stats.Copy(&base_stats);
    return copy;
}

void StatBlock::Clear(){
    level=0;
    base_stats.Clear();
}

