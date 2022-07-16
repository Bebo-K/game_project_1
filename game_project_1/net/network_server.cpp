#include <game_project_1/net/network.hpp>
#include <game_project_1/net/os_net.hpp>
#include <game_project_1/net/packets.hpp>

#include <game_project_1/types/data_types.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/os.hpp>

#include <game_project_1/client/client.hpp>
#include <game_project_1/server/server_net_handler.hpp>
#include <game_project_1/server/server_signal.hpp>



bool ServerNetwork::running=false;
bool ServerNetwork::listener_enabled=false;
bool ServerNetwork::local_listener=false;
Socket ServerNetwork::listener_socket=INVALID_SOCKET;
unsigned short ServerNetwork::listener_port=0;

NetTarget* ServerNetwork::targets=nullptr;

int ServerNetwork::target_count=0;
int ServerNetwork::connected_targets=0;

const int POLL_INTERVAL_MS = 10;

using namespace ServerNetwork;

void ServerNetworkThreadEntryPoint(){
    long loop_start;
    long loop_delta;
    Datagram packet_buffer;
    NetAddress remote_address;

    bool cleanup_loop=true;
    NetTarget* target= nullptr;
    int target_start_index=0;

    if(listener_enabled){
        listener_socket = OSNetwork::BindToPort(listener_port);
    }
    if(local_listener){//Local is push/push mode. Server pushes msgs to client target instance and client pushes packets from server target instance. 
        target_start_index++;
        targets[0].ForLocal(&ClientNetwork::GetNetTargetForLocal()->inbound_buffer);
    }

    while(running||cleanup_loop){   if(!running){cleanup_loop=false;}
        loop_start=OS::time_ms();
        for(int i=0;i<target_count;i++){
            target = &targets[i];
            if(target->IsConnected()){
                while(target->outbound_buffer.Read((byte*)&packet_buffer)){
                    target->connection.Write(&packet_buffer);
                }
                while(target->connection.Read(&packet_buffer)){
                    target->inbound_buffer.Write((byte*)&packet_buffer);
                    target->last_success = OS::time_ms();
                }
            }
            else if(target->IsJustDisconnected()){
                target->Clear();
            }
        }

        if(listener_enabled){
            int listen_result = OSNetwork::Listen(&packet_buffer,listener_socket,&remote_address);
            if(listen_result < 0){listener_enabled=false;}
            while(listen_result > 0){
                bool target_exists = false;
                for(int i=target_start_index;i < target_count; i++){
                    if(targets[i].connection.address.Matches(&remote_address)){
                        targets[i].inbound_buffer.Write((byte*)&packet_buffer);
                        targets[i].last_success = OS::time_ms();
                        target_exists=true;
                        //Breakpoint on this as once a target is accepted it shouldn't happen.
                        break;
                    }
                }
                if(!target_exists){
                    ServerNetwork::HandleNewConnection(packet_buffer.ToPayload(),remote_address);
                }
                listen_result = OSNetwork::Listen(&packet_buffer,listener_socket,&remote_address);
                if(listen_result < 0){
                    logger::exception("Server listener failed with error code %d\n",listen_result);
                    listener_enabled=false;
                }
            }
        }
        else if(listener_socket != INVALID_SOCKET){
            OSNetwork::Unbind(&listener_socket);
        }
        loop_delta = OS::time_ms()-loop_start;
        if(loop_delta > POLL_INTERVAL_MS){loop_delta = POLL_INTERVAL_MS;}
        if(loop_delta < 0){
            logger::exception("Server listener poll took negative time? (%d ms)\n",loop_delta);
        }
        OS::SleepThread(POLL_INTERVAL_MS-loop_delta);
    }

    //Cleanup
    packet_buffer.FromPayload(Packet::NOPE(L"Server is shutting down").GetPayload());

    if(local_listener && ClientNetwork::IsRunning()){//Unplug client local link
        ClientNetwork::GetNetTargetForLocal()->connection.Disconnect();
    }

    for(int i=0;i<target_count;i++){
        if(targets[i].connection.IsConnected()){
            targets[i].connection.Write(&packet_buffer);
            targets[i].connection.Disconnect();
        }
        targets[i].~NetTarget();
    }


    free(targets);
    targets=nullptr;
}

void ServerNetwork::Init(){
    running=false;
    listener_enabled=false;
    listener_socket=INVALID_SOCKET;
    listener_port=0;

    targets=nullptr;

    target_count=0;
    connected_targets=0;
}
void ServerNetwork::Free(){
    ShutdownListener();
}
bool ServerNetwork::IsRunning(){return running;}
NetTarget* ServerNetwork::GetNetTargetForLocal(){return &targets[0];}

void ServerNetwork::StartListener(int player_slots,bool include_local,unsigned short port){
    target_count=player_slots;

    targets = (NetTarget*)calloc(target_count,sizeof(NetTarget));
    for(int i=(include_local?1:0);i<target_count;i++){
        new (&targets[i]) NetTarget();
        targets[i].Clear();
    }

    if(include_local){targets[0].connection.address.SetLocalhost(true);local_listener=true;}

    running=true;
    listener_port=port;
    listener_enabled=true;
    OS::StartThread(ServerNetworkThreadEntryPoint);
}
void ServerNetwork::StartLocalOnly(){
    target_count=1;
    targets = new NetTarget();
    targets->connection.address.SetLocalhost(true);
    local_listener=true;

    running=true;
    OS::StartThread(ServerNetworkThreadEntryPoint);
}

void ServerNetwork::Update(){
    if(running){
        for(int i=0;i<target_count;i++){
            if(targets[i].connection.state < 0){
                ServerNetHandler::OnPlayerDisconnect(i,targets[i].connection.message);
                targets[i].Clear();
            }
            if(!targets[i].IsConnected())continue;
            targets[i].Update();
        }
    }
}

void ServerNetwork::ShutdownListener(){
    running=false;
    listener_enabled=false;
    listener_port=0;
    int join_timeout = 5000;
    while(targets != nullptr && join_timeout > 0){join_timeout--;OS::SleepThread(1);}//join with listener thread
    //TODO: join timeout error msg
}
bool ServerNetwork::PlayerConnected(int player_slot){
    return targets[player_slot].IsConnected();
}

void ServerNetwork::SendToPlayer(int player_slot,Payload payload){
    if(targets[player_slot].IsConnected())targets[player_slot].Send(payload);
}
void ServerNetwork::SendToAllPlayers(Payload payload){
    for(int i=0;i<target_count;i++){
        if(targets[i].IsConnected())targets[i].Send(payload);
    }
}
void ServerNetwork::SendToOtherPlayers(Payload payload,int excluded_player){
    for(int i=0;i<target_count;i++){
        if(i != excluded_player && targets[i].IsConnected())targets[i].Send(payload);
    }
}
Payload ServerNetwork::RecvFromPlayer(int player_slot){
    return targets[player_slot].Recieve();
}


void RejectResponse(wchar* response,NetAddress address){
    Datagram dgram;
        dgram.FromPayload(Packet::NOPE(response).GetPayload());
    OSNetwork::Ping(&dgram,address);
}

void InfoResponse(NetAddress address){
    Packet::SINF server_info;
        server_info.players=connected_targets;
        server_info.max_players=target_count;
        server_info.SetNameAndDescription(server_config::server_name,server_config::server_description);
        
    Datagram dgram;
        dgram.FromPayload(server_info.GetPayload());
    OSNetwork::Ping(&dgram,address);
}

int ServerNetwork::JoinPlayer(NetAddress address){ 
    if(local_listener && address.IsLocalhost()){
        targets[0].connection.address = address;
        targets[0].connection.address.SetLocalhost(true);
        targets[0].connection.SetState(Connection::CONNECTED,nullptr);
        targets[0].last_success = OS::time_ms();
        return 0;
    }

    for(int i=(local_listener?1:0);i<target_count;i++){
        if(targets[i].connection.state == Connection::NOT_CONNECTED){
            targets[i].connection.SetState(Connection::CONNECTED,nullptr);
            targets[i].connection.address = address;
            targets[i].last_success = OS::time_ms();
            return i;
        }
    }
    return -1;
}

void ServerNetwork::HandleNewConnection(Payload request,NetAddress remote_address){
    switch(request.type){
        case PacketID::JOIN: {
            if(JoinPlayer(remote_address) < 0){
                Packet::JOIN join_request(request);
                wchar server_full_msg[] = L"Server is full";
                Packet::PLDC dc_notification;
                    dc_notification.player_id = -1;
                    dc_notification.SetPlayerAndReason(join_request.persona_buffer,server_full_msg);
                SendToAllPlayers(dc_notification.GetPayload());
                RejectResponse(server_full_msg,remote_address);
            }
            break;
        }
        case PacketID::INFO: {
            InfoResponse(remote_address);
            break;
        }
        default: {
            RejectResponse(L"Bad Request",remote_address);
        }
    }
}

void ServerNetwork::DisconnectPlayer(int player_slot,wchar* reason){    
    ServerNetHandler::OnPlayerDisconnect(player_slot,reason);
    logger::infoW(L"Client id %d disconnected (%S)",player_slot,reason);
    if(targets[player_slot].IsConnected()){targets[player_slot].SendDisconnect(reason);}
}
