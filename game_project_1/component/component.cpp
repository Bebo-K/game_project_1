#include <game_project_1/component/component.hpp>
#include <game_project_1/os.hpp>


void Component::MarkUpdated(int timestamp){last_updated=timestamp;}
void Component::MarkUpdated(){last_updated=OS::time_ms();}

