#include <game_project_1/system/npc_controller.hpp>


Map<NPCControllerType,NPCControllerCallback> npc_controllers(4);
void EmptyNPCControllerCallback(ServerEntity* e, ServerScene* s){}

void NPCController::FrameUpdate(ServerEntity* e,ServerScene* s){
    if(!e->Has<NPCState>()){return;}
    NPCState npc_state = e->ServerGet<NPCState>();
    NPCControllerCallback callback = GetNPCControllerCallback(npc_state->controller_type);
    callback(e,s);
}
void NPCController::RegisterNPCControllerCallback(NPCControllerType type,NPCControllerCallback callback){
    npc_controllers.Add(type,callback);
}

NPCControllerCallback NPCController::GetNPCControllerCallback(NPCControllerType type){
    return (npc_controllers.Has(type))? npc_controllers.Get(type) : &EmptyNPCControllerCallback;
}