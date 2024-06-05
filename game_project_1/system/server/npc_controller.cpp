#include <game_project_1/system/server/npc_controller.hpp>
#include <game_project_1/types/map.hpp>

#include <game_project_1/component/server/npc_properties.hpp>

Dictionary<NPCControllerType,NPCControllerCallback> npc_controllers(4);
void EmptyNPCControllerCallback(ServerEntity* e, ServerScene* s){}

void NPCController::FrameUpdate(ServerEntity* e,ServerScene* s){
    if(!e->Has<NPCProperties>()){return;}
    NPCProperties* npc_state = e->Get<NPCProperties>();
    NPCControllerCallback callback = GetNPCControllerCallback(npc_state->controller_type);
    callback(e,s);
}
void NPCController::RegisterNPCControllerCallback(NPCControllerType type,NPCControllerCallback callback){
    npc_controllers.Add(type,callback);
}

NPCControllerCallback NPCController::GetNPCControllerCallback(NPCControllerType type){
    return (npc_controllers.Has(type))? npc_controllers.Get(type) : &EmptyNPCControllerCallback;
}