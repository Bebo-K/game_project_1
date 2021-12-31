#include "network.h"
#include "os_net.h"
#include "packet_builder.h"

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"

#include "../game.h"


using namespace Network;

void ServerSignalDisconnect(int player_id,wchar* reason){

}
void ServerSignalFailConnect(wchar* name,wchar* reason){

}
void ServerSignalPlayerConnect(wchar* name,int player_id){

}

void ServerNetworkThreadEntryPoint(void* network_ptr){
    ServerNetwork* net=(ServerNetwork*)network_ptr;

    Packet transfer_packet;
    ip_address remote_address;

    bool cleanup_loop=true;

    while(net->running||cleanup_loop){
        if(!net->running){cleanup_loop=false;}
        for(NetTarget* target: net->targets){
            if(target->IsConnected()){
                while(target->outbound_buffer.Read((byte*)&transfer_packet)){
                    OSNetwork::send_packet(&transfer_packet,target);
                }
                while(OSNetwork::recv_packet(&transfer_packet,target)){
                    target->inbound_buffer.Write((byte*)&transfer_packet);
                    target->last_recv = time_ms();
                    if(target->state_id != CONNECTED){
                        target->SetState(CONNECTED,nullptr);
                    }
                }
            }
            if(target->state_id < 0 || target->state_id == DISCONNECTING){
                if(target->conn_start == -1){
                    ServerSignalDisconnect(net->targets.IndexOf(target),target->state_msg);
                    OSNetwork::disconnect(target);
                    target->Clear();
                }
                else{target->conn_start=-1;}//use conn_start to flag one more loop to process disconnect msgs, then cleanup.
            }
        }

        if(net->local_client_target.IsConnected()){
            if(Game::client != nullptr && Game::client->network.running){
                while(net->local_client_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    net->local_client_target.inbound_buffer.Write((byte*)&transfer_packet);
                }
                while(net->local_client_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    net->local_client_target.inbound_buffer.Write((byte*)&transfer_packet);
                    net->local_client_target.last_recv = time_ms();
                }
            }else{
                net->local_client_target.Clear();
            }
        }

        if(net->listener_enabled){
            if(net->listener_socket == INVALID_SOCKET){//Startup listener
                net->listener_socket = OSNetwork::bind_to_port(net->listener_port);
                if(net->listener_socket == INVALID_SOCKET){
                    net->listener_enabled=false;
                }
            }
            else{
                int listen_result = OSNetwork::listen(&transfer_packet,net->listener_socket,&remote_address);
                if(listen_result < 0){net->listener_enabled=false;}
                while(listen_result > 0){
                    bool target_exists = false;
                    for(NetTarget* target: net->targets){
                        if(target->address.Matches(&remote_address)){
                            target->inbound_buffer.Write((byte*)&transfer_packet);
                            target->last_recv = time_ms();
                            if(target->state_id != CONNECTED){
                                target->SetState(CONNECTED,nullptr);
                            }
                            target_exists=true;
                            break;
                        }
                    }
                    if(!target_exists){
                        net->HandleNewTarget(&transfer_packet,remote_address);
                    }
                    listen_result = OSNetwork::listen(&transfer_packet,net->listener_socket,&remote_address);
                    if(listen_result < 0){net->listener_enabled=false;}
                }
            }
        }
        else if(net->listener_socket != INVALID_SOCKET){
            OSNetwork::unbind(&net->listener_socket);
        }
        sleep_thread(10);
    }
}

ServerNetwork::ServerNetwork(){
    running=false;
    listener_enabled=false;
    listener_socket=INVALID_SOCKET;
    listener_port=0;
    local_client_target.Clear();
    local_client_target.address.SetLocalhost(true);
}
ServerNetwork::~ServerNetwork(){
    ShutdownListener();
}
void ServerNetwork::StartListener(int target_slots,unsigned short port){
    targets.Resize(target_slots);
    for(NetTarget* target: targets){
        target->Clear();
    }
    running=true;
    listener_port=port;
    listener_enabled=true;
    start_thread(ServerNetworkThreadEntryPoint,this);
}
void ServerNetwork::StartLocalOnly(){
    running=true;
    start_thread(ServerNetworkThreadEntryPoint,this);
}
void ServerNetwork::Update(){
    for(NetTarget* target:targets){
        if(!target->IsConnected())continue;
        target->Update();
    }
}
void ServerNetwork::ShutdownListener(){
    for(NetTarget* target:targets){
        if(target->IsConnected()){target->Disconnect(L"Server is shutting down");}
    }
    running=false;
    listener_enabled=false;
    listener_port=0;
}
bool ServerNetwork::TargetConnected(int target_id){
    if(local_client_target.IsConnected()){
        if(target_id==0){return true;}
        else target_id -=1;
    }
    return targets[target_id]->IsConnected();
}

void ServerNetwork::WriteToAllTargets(Payload payload){
    if(local_client_target.IsConnected()){local_client_target.Send(payload);}
    for(NetTarget* target: targets){
        if(target->IsConnected()){target->Send(payload);}
    }
}
void ServerNetwork::WriteToTarget(int target_id,Payload payload){
    if(local_client_target.IsConnected()){
        if(target_id==0){local_client_target.Send(payload);return;}
        else target_id -=1;
    }
    if(target_id < 0 || target_id > targets.length){logger::exception("ServerNetwork.WriteToTarget: Invalid target ID: %d", target_id);}
    if(targets[target_id]->IsConnected()){targets[target_id]->Send(payload);}
}
Payload ServerNetwork::ReadFromTarget(int target_id){
    if(local_client_target.IsConnected()){
        if(target_id==0){return local_client_target.Recieve();}
        else target_id -=1;
    }
    if(target_id < 0 || target_id > targets.length){logger::exception("ServerNetwork.ReadFromTarget: Invalid target ID: %d", target_id);}
    if(targets[target_id]->IsConnected()){return targets[target_id]->Recieve();}
    return Payload(0,0,nullptr);
}
void ServerNetwork::HandleNewTarget(Packet* request_packet,ip_address remote_address){
    if(request_packet->type != PacketID::JOIN){
        NetTarget temporary_target;
        Packet reject_packet = BuildPacket_NOPE(L"Bad request");
        temporary_target.SetAddress(remote_address);
        OSNetwork::connect(&reject_packet,&temporary_target);
        OSNetwork::disconnect(&temporary_target);
        return;
    }
    
    int max_clients = server_config::player_count;
    bool accepted=false;
    if(local_client_target.IsConnected()){max_clients--;}
    if(targets.length < max_clients){max_clients= targets.length;}
    for(int i=0;i<max_clients;i++){
        if(targets[i]->state_id == NOT_CONNECTED){
            NetTarget* new_target = targets[i];
            new_target->SetState(CONNECTING,nullptr);
            new_target->address = remote_address;
            new_target->conn_start = time_ms();
            new_target->last_recv = time_ms();

            Packet accept_packet = BuildPacket_ACPT(request_packet->id,i);
            OSNetwork::connect(&accept_packet,new_target);
            accepted=true;
            ServerSignalPlayerConnect(ReadPacket_JOIN_name(request_packet),i);
        }
    }
    if(!accepted){
        NetTarget temporary_target;
        Packet reject_packet = BuildPacket_NOPE(L"Server is full.");
        temporary_target.SetAddress(remote_address);
        OSNetwork::connect(&reject_packet,&temporary_target);
        OSNetwork::disconnect(&temporary_target);
        ServerSignalFailConnect(ReadPacket_JOIN_name(request_packet),L"Server is full");
    }
}
void ServerNetwork::DisconnectTarget(int target_id,wchar* reason){
    if(target_id < 0 || target_id > targets.length){logger::exception("ServerNetwork.DisconnectTarget: Invalid player ID: %d", target_id);}
    if(targets[target_id]->IsConnected()){
        targets[target_id]->Disconnect(reason);
    }
}