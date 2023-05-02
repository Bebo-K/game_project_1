#include <game_project_1/component/server/persistence.hpp>





Persistence::Persistence(SaveEntity* e){save_entity=e;}
Persistence::~Persistence(){}
Component* Persistence::Clone(){return new Persistence(save_entity);}
void Persistence::Clear(){save_entity=null;}