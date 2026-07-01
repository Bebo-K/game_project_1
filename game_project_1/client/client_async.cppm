export module ClientAsync;

import Client;
import ClientNetwork;
import Server;
import Logger;
import String;
import Config;
import Memory;

import ClientEntity;
import LocalPlayer;

namespace ClientAsync {

    struct DisconnectEvent {
        bool raised;
        int disconnect_code;
        wchar* disconnect_reason;
    };

    struct LocalServerReadyEvent {
        bool raised;
    };

    struct SpawnPlayerEvent {
        bool raised;
        ClientEntity* entity;
    };

    static DisconnectEvent disconnect_event = {false,0,nullptr};
    static LocalServerReadyEvent local_server_ready_event = {false};
    static SpawnPlayerEvent spawn_player_event = {false,nullptr};


    export void Disconnect(int disconnect_code,wchar* disconnect_reason) {
        disconnect_event = {true,disconnect_code,disconnect_reason};
    }
    export void LocalServerReady() {
        local_server_ready_event.raised = true;
    }
    export bool SpawnPlayer(ClientEntity* entity) {
        spawn_player_event = {true,entity};
    }

    void HandleDisconnect() {
        Client* instance = Client::GetClient();
        Server* server = Server::GetServer();
        if(!instance)return;
        if(disconnect_event.disconnect_code < 0){
            instance->ui.CloseAll();
            instance->ui.error_menu->Open();
            instance->scene.Unload();
            if(disconnect_event.disconnect_reason){
                logger::infoW(L"Disconnected from server (%S)\n",disconnect_event.disconnect_reason);
                instance->ui.error_menu->SetStatusMessage(wstr::allocf(L"Disconnected from server (%S)",disconnect_event.disconnect_reason));
                Free(disconnect_event.disconnect_reason);
            }
            else{
                logger::infoW(L"Disconnected from server (No reason given)\n");
                instance->ui.error_menu->SetStatusMessage(wstr::new_copy(L"Disconnected from server (No reason given)"));
            }
        }
        else{
            instance->ui.CloseAll();
            instance->ui.main_menu->Open();
            instance->scene.Unload();
        }
        if(server){server->StartShutdown();}
    }

    void HandleLocalServerReady(){
        Client* instance = Client::GetClient();
        if(!instance)return;
        logger::info("Local server is ready. Connecting...\n");
        instance->ui.loading_menu->SetStatusMessage(wstr::new_copy(L"Connecting to local server..."));
        ClientNetwork::LocalConnect(config::player_persona, LocalPlayer::GetCache()->my_save_id);
    }

    void HandleSpawnPlayer() {
        Client* instance = Client::GetClient();
        if(!instance)return;
        logger::debug("Setting player control to entity %d\n",spawn_player_event.entity->id);
        instance->scene.SetPlayerControl(spawn_player_event.entity->id);
        //UI TransitionTo() ?
        if(!instance->ui.ingame_menu->active){instance->ui.ingame_menu->Open();}
        if(instance->ui.loading_menu->active){instance->ui.loading_menu->Close();}
    }

    export void Poll() {
        if (disconnect_event.raised){HandleDisconnect();disconnect_event.raised=false;}
        if (local_server_ready_event.raised){HandleLocalServerReady(); local_server_ready_event.raised=false;}
        if (spawn_player_event.raised){HandleSpawnPlayer(); spawn_player_event.raised=false;}
    }
}