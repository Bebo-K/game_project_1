#include <game_project_1/component/server/persistence.hpp>

Persistence::Persistence(){global_id = -1;}
Persistence::Persistence(int guid){global_id = guid;}
Persistence::~Persistence(){global_id=-1;}
Component* Persistence::Clone(){return new Persistence(global_id);}
void Persistence::Clear(){global_id=-1;}