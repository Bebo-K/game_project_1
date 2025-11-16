#include <game_project_1/component/server/player_owner.hpp>


PlayerOwner::PlayerOwner(){player_id=-1;}
PlayerOwner::PlayerOwner(int player){player_id=player;}
PlayerOwner::~PlayerOwner(){}

Component* PlayerOwner::Clone(ComponentParentContext context){return new PlayerOwner(player_id);}
void PlayerOwner::Clear(){player_id = -1;}
void PlayerOwner::Read(Deserializer& dat){}
void PlayerOwner::Write(Serializer& dat){}
