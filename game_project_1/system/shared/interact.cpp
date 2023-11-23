#include <game_project_1/system/shared/interact.hpp>

#include <game_project_1/gui/gui.hpp>


void ClientOnTalkInteraction(ClientEntity* e1,ClientEntity* e2, ClientScene* s){
    GUI* ui = GUI::GetGUI();
    int dialogue_id = e2->Get<Interactable>()->interaction_content_id;
    ui->ingame_menu->StartDialogueBox(dialogue_id);
}

/*
void ClientAfterTalkInteraction(ClientEntity* e1,ClientEntity* e2, ClientScene* s){}
void ServerOnTalkInteraction(ServerEntity* e1,ServerEntity* e2, ServerScene* s){}
void ServerAfterTalkInteraction(ServerEntity* e1,ServerEntity* e2, ServerScene* s){}
*/

ClientEntity* Interact::ClientPollInteract(ClientEntity* e,ClientScene* s){
    for(ClientEntity* e2: s->entities){
        if(e2 == e)continue;
        if(e2->Has<Interactable>()){
            Interactable* e2_interact = e2->Get<Interactable>();
            vec3 offset = (e2->GetPos() - e->GetPos());
            float dist = offset.length_sqr();
            if(dist < e2_interact->interact_range){
                //float interact_angle = offset.xz_angle()-90.0-e2->rotation.y;
                //if(e2_interact->valid_interact_angle.Contains(interact_angle)){...
                return e2;//todo- interact list?
            }
        }
    }
    return nullptr;
}

/*
bool Interact::ServerVerifyInteract(ServerEntity* e,ServerEntity* e2,ServerScene* s){
    if(e2->Has<Interactable>()){
        Interactable* e2_interact = e2->Get<Interactable>();
        vec3 offset = (e2->GetPos() - e->GetPos());
        float dist = offset.length_sqr();
        if(dist < e2_interact->interact_range){
            //float interact_angle = offset.xz_angle()-90.0-e2->rotation.y;
            //if(e2_interact->valid_interact_angle.Contains(interact_angle)){...
            return true;
        }
    }
    return false;
}
*/

void Interact::ClientTryInteract(ClientEntity* e,ClientEntity* e2,ClientScene* s){
    Interactable* e2_interact = e2->Get<Interactable>();
    
    switch(e2_interact->type){
        case InteractionType::PICKUP: break;
        case InteractionType::TALK: {
            ClientOnTalkInteraction(e,e2,s);    
            break;
        }
        case InteractionType::CUSTOM: break;
        default:break;
    }
    //SignalServerInteraction(e,e2,s);
}

void Interact::ServerTryInteract(ServerEntity* e,ServerEntity* e2,ServerScene* s){
    Interactable* e2_interact = e2->Get<Interactable>();
    
    switch(e2_interact->type){
        case InteractionType::PICKUP: {break;}
        case InteractionType::TALK: {break;}
        case InteractionType::CUSTOM: {break;}
        default:break;
    }
    //SignalClientInteraction(e,e2,s);
}