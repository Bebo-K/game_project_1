#include "network.h"
#include "os_net.h"
#include "packet_builder.h"
#include "../server/server_signal.h"

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"

#include "../client/client.h"
#include "../server/server_net_handler.h"




bool ServerNetwork::running=false;

bool ServerNetwork::listener_enabled=false;
Socket ServerNetwork::listener_socket=INVALID_SOCKET;
unsigned short ServerNetwork::listener_port=0;

NetTarget* ServerNetwork::targets=nullptr;
NetTarget* ServerNetwork::local_client_target=nullptr; //will be targets[0] if allowing local connections, otherwise null

int ServerNetwork::target_count=0;
int ServerNetwork::connected_targets=0;

using namespace ServerNetwork;

void ServerNetworkThreadEntryPoint(){
    long loop_start;
    long loop_delta;
    Packet transfer_packet;
    ip_address remote_address;

    bool cleanup_loop=true;
    NetTarget* target= nullptr;

    int remote_target_start_index = (local_client_target == targets)?1:0;

    while(running||cleanup_loop){
        loop_start=time_ms();
        if(!running){cleanup_loop=false;}
        for(int i=remote_target_start_index; i<target_count;i++){
            target = &targets[i];
            if(target->IsConnected()){
                while(target->outbound_buffer.Read((byte*)&transfer_packet)){
                    OSNetwork::send_packet(&transfer_packet,target);
                }
                while(OSNetwork::recv_packet(&transfer_packet,target)){
                    target->inbound_buffer.Write((byte*)&transfer_packet);
                    target->last_recv = time_ms();
                    if(target->state_id != NetTargetState::ID::CONNECTED){
                        target->SetState(NetTargetState::ID::CONNECTED,nullptr);
                    }
                }
            }
            if(target->state_id < 0 || target->state_id == NetTargetState::ID::DISCONNECTING){
                if(target->conn_start == -1){
                    ServerNetHandler::OnPlayerDisconnect(target->state_msg,i);
                    OSNetwork::disconnect(target);
                    target->Clear();
                }
                else{target->conn_start=-1;}//use conn_start to flag one more loop to process disconnect msgs, then cleanup.
            }
        }
        if(local_client_target != nullptr){//Local is pull/pull mode. Server pulls msgs from client target instance and client pulls packets from server target instance. 
            if(ClientNetwork::IsRunning()){
                target = ClientNetwork::GetNetTargetForLocal();
                while(target->outbound_buffer.Read((byte*)&transfer_packet)){
                    if(local_client_target->IsConnected()){
                        local_client_target->inbound_buffer.Write((byte*)&transfer_packet);
                        local_client_target->last_recv = time_ms();
                    }
                    else{
                        ServerNetwork::HandleNewLocalTarget(&transfer_packet);
                    }
                }
            }
        }

        if(listener_enabled){
            if(listener_socket == INVALID_SOCKET){//Startup listener
                listener_socket = OSNetwork::bind_to_port(listener_port);
                if(listener_socket == INVALID_SOCKET){
                    listener_enabled=false;
                }
            }
            else{
                int listen_result = OSNetwork::listen(&transfer_packet,listener_socket,&remote_address);
                if(listen_result < 0){listener_enabled=false;}
                while(listen_result > 0){
                    bool target_exists = false;
                    for(int i=remote_target_start_index;i < target_count; i++){
                        target  = &targets[i];
                        if(target->address.Matches(&remote_address)){
                            target->inbound_buffer.Write((byte*)&transfer_packet);
                            target->last_recv = time_ms();
                            if(target->state_id != NetTargetState::ID::CONNECTED){
                                target->SetState(NetTargetState::ID::CONNECTED,nullptr);
                            }
                            target_exists=true;
                            break;
                        }
                    }
                    if(!target_exists){
                        ServerNetwork::HandleNewTarget(&transfer_packet,remote_address);
                    }
                    listen_result = OSNetwork::listen(&transfer_packet,listener_socket,&remote_address);
                    if(listen_result < 0){listener_enabled=false;}
                }
            }
        }
        else if(listener_socket != INVALID_SOCKET){
            OSNetwork::unbind(&listener_socket);
        }
        loop_delta = time_ms()-loop_start;
        if(loop_delta < 10 && loop_delta >= 0)sleep_thread(10-loop_delta);
    }

    //Cleanup
    PacketData::NOPE disconnect_packet(&transfer_packet);
        disconnect_packet.SetReason(L"Server is shutting down");
        disconnect_packet.WritePacket();
    for(int i=0;i<target_count;i++){
        if(targets[i].IsConnected()){
            OSNetwork::send_packet(&transfer_packet,&targets[i]);
            OSNetwork::disconnect(&targets[i]);
        }
        targets[i].~NetTarget();
    }
    if(local_client_target == targets){local_client_target = nullptr;}
    free(targets);
    targets=nullptr;
}

void ServerNetwork::Init(){
    running=false;
    listener_enabled=false;
    listener_socket=INVALID_SOCKET;
    listener_port=0;

    targets=nullptr;
    local_client_target=nullptr;

    target_count=0;
    connected_targets=0;
}
void ServerNetwork::Free(){
    ShutdownListener();
}
bool ServerNetwork::IsRunning(){return running;}
NetTarget* ServerNetwork::GetNetTargetForLocal(){return local_client_target;}
void ServerNetwork::StartListener(int target_slots,bool allow_local,unsigned short port){
    target_count=target_slots;

    targets = (NetTarget*)calloc(target_slots,sizeof(NetTarget));
    for(int i=0;i<target_count;i++){
        new (&targets[i]) NetTarget();
        targets[i].Clear();
    }

    if(allow_local){
        local_client_target = targets;
        local_client_target->ForLocal();
    }

    running=true;
    listener_port=port;
    listener_enabled=true;
    start_thread(ServerNetworkThreadEntryPoint);
}
void ServerNetwork::StartLocalOnly(){
    target_count=1;
    targets = (NetTarget*)calloc(1,sizeof(NetTarget));
    local_client_target = new (targets) NetTarget();
    local_client_target->ForLocal();

    running=true;
    start_thread(ServerNetworkThreadEntryPoint);
}
void ServerNetwork::Update(){
    if(running){
        for(int i=0;i<target_count;i++){
            if(!targets[i].IsConnected())continue;
            targets[i].Update();
        }
    }
}
void ServerNetwork::ShutdownListener(){
    running=false;
    listener_enabled=false;
    listener_port=0;
    while(targets != nullptr);//join with listener thread
}
bool ServerNetwork::TargetConnected(int target_id){
    return targets[target_id].IsConnected();
}
void ServerNetwork::SendToTarget(int target_id,Payload payload){
    if(targets[target_id].IsConnected())targets[target_id].Send(payload);
}
void ServerNetwork::SendToTarget(int target_id,Packet* packet){
    if(targets[target_id].IsConnected())targets[target_id].Send(packet);
}
void ServerNetwork::SendToAllTargets(Payload payload){
    for(int i=0;i<target_count;i++){
        if(targets[i].IsConnected())targets[i].Send(payload);
    }
}
void ServerNetwork::SendToAllTargets(Packet* packet){
    for(int i=0;i<target_count;i++){
        if(targets[i].IsConnected())targets[i].Send(packet);
    }
}
void ServerNetwork::SendToOtherTargets(Payload payload,int excluded_target){
    for(int i=0;i<target_count;i++){
        if(i != excluded_target && targets[i].IsConnected())targets[i].Send(payload);
    }
}
void ServerNetwork::SendToOtherTargets(Packet* packet,int excluded_target){
    for(int i=0;i<target_count;i++){
        if(i != excluded_target && targets[i].IsConnected())targets[i].Send(packet);
    }
}
Payload ServerNetwork::RecvFromTarget(int target_id){
    if(targets[target_id].IsConnected()){return targets[target_id].Recieve();}
    return Payload(0,0,nullptr);
}
void ServerNetwork::HandleNewTarget(Packet* request_packet,ip_address remote_address){
    Packet response;
    NetTarget* accept_target=nullptr;

    if(request_packet->type != PacketID::JOIN){
        PacketData::NOPE deny_response(&response);
            deny_response.SetReason(L"Bad Request");
            deny_response.WritePacket();
    }
    else{
        bool accepted=false;
        for(int i=1;i<target_count;i++){
            if(targets[i].state_id == NetTargetState::ID::NOT_CONNECTED){
                NetTarget* new_target = &targets[i];
                new_target->SetState(NetTargetState::ID::CONNECTING,nullptr);
                new_target->address = remote_address;
                new_target->conn_start = time_ms();
                new_target->last_recv = time_ms();
                response = ServerNetHandler::OnPlayerConnect(request_packet,i);

                accepted=true;
                connected_targets++;
            }
        }
        if(!accepted){
            PacketData::NOPE deny_response(&response);
                deny_response.SetReason(L"Server is full.");
                deny_response.WritePacket();
                ServerNetHandler::OnPlayerFailConnect(PacketData::JOIN(request_packet).GetPlayerName(),deny_response.GetReason());
        }
    }    
    if(accept_target != nullptr && response.type != PacketID::NOPE){
        accept_target->AddToReliableBuffer(&response);
        OSNetwork::connect(&response,accept_target);
    }
    else{
        NetTarget temporary_target;
            temporary_target.SetAddress(remote_address);
        OSNetwork::connect(&response,&temporary_target);
        OSNetwork::disconnect(&temporary_target);
    }
}

void ServerNetwork::HandleNewLocalTarget(Packet* request_packet){
    //assumes local target exists. Local clients can't get kicked out of their own servers and likely won't send itself garbage
    local_client_target->last_recv = time_ms();
    local_client_target->SetState(NetTargetState::ID::CONNECTED_LOCAL,nullptr);
    Packet response = ServerNetHandler::OnPlayerConnect(request_packet,0);
    local_client_target->AddToReliableBuffer(&response);
    OSNetwork::connect(&response,local_client_target);
    connected_targets++;
}
void ServerNetwork::DisconnectTarget(int target_id,wchar* reason){
    logger::infoW(L"Client id %d disconnected (%S)",target_id,reason);
    if(targets[target_id].IsConnected()){targets[target_id].Disconnect(reason);}
}