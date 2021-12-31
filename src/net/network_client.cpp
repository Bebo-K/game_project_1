#include "network.h"
#include "os_net.h"
#include "packet_builder.h"

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"

#include "../game.h"


void ClientNetworkThreadEntryPoint(void* network_ptr){
    ClientNetwork* net=(ClientNetwork*)network_ptr;

    Packet transfer_packet;
    bool cleanup_loop=false;
    
    while(net->running){
        if(net->local_only){
            if(Game::server != nullptr && Game::server->network.running){
                while(net->server_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    Game::server->network.local_client_target.inbound_buffer.Write((byte*)&transfer_packet);
                }
                while(Game::server->network.local_client_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    net->server_target.inbound_buffer.Write((byte*)&transfer_packet);
                    net->server_target.last_recv = time_ms();
                }
            }
        }
        else if(net->server_target.state_id == NetTarget::CONNECTING || net->server_target.IsConnected()){
           while(net->server_target.outbound_buffer.Read((byte*)&transfer_packet)){
                OSNetwork::send_packet(&transfer_packet,&net->server_target);
            }
            while(OSNetwork::recv_packet(&transfer_packet,&net->server_target)){
                net->server_target.inbound_buffer.Write((byte*)&transfer_packet);
                net->server_target.last_recv = time_ms();
                net->server_target.state_id = NetTarget::CONNECTED;
            }
        }
        if(net->server_target.state_id == NetTarget::DISCONNECTING){
            if(cleanup_loop){
                OSNetwork::disconnect(&net->server_target);
                net->server_target.Clear();
                net->running=false;
            }
            cleanup_loop=true;
        }
        sleep_thread(10);
    }
}


ClientNetwork::ClientNetwork():server_target(){
    running=false;
    local_only=false;
    network_state=ClientNetwork::NO_CONNECTION;
    network_substatus=nullptr;
    connection_start=0;
    last_ping=0;
}
ClientNetwork::~ClientNetwork(){
    running=false;
    local_only=false;
}
void ClientNetwork::StartConnect(wchar* host_string,Packet* JOIN){
    local_only=false;
    server_target.SetAddress(host_string);

    server_target.AddToReliableBuffer(JOIN);
    server_target.outbound_buffer.Write((byte*)JOIN);
    start_thread(ClientNetworkThreadEntryPoint,this);
    connection_start=time_ms();
    last_ping=time_ms();
    SetNetworkState(CONNECTING,wstr::allocf(L"Connecting to %s ...",host_string));
}
void ClientNetwork::LocalConnect(Packet* JOIN){
    local_only=true;
    server_target.ForLocal();
    server_target.AddToReliableBuffer(JOIN);
    server_target.outbound_buffer.Write((byte*)JOIN);
    start_thread(ClientNetworkThreadEntryPoint,this);
    connection_start=time_ms();
    last_ping=time_ms();
    SetNetworkState(CONNECTING,wstr::new_copy(L"Connecting to local server..."));
}
void ClientNetwork::Disconnect(wchar* reason){ 
    server_target.Disconnect(reason);
    connection_start=0;
    last_ping=0;
    SetNetworkState(NO_CONNECTION,reason);
}
void ClientNetwork::Update(){
    if(!server_target.IsConnected())return;
    server_target.Update();
    
    if(server_target.IsConnected() && network_state != CONNECTED){SetNetworkState(CONNECTED,nullptr);}
    if(server_target.state_id == NetTarget::NOT_CONNECTED && network_state == CONNECTED){
        SetNetworkState(CONNECTION_ERROR,wstr::allocf(L"Disconnected by server: %s",server_target.state_msg));
    }

    //Ping initiation
    if(time_ms() - last_ping > config::network_ping_interval){
        Packet ping = BuildPacket_PING();
        Send(&ping);
        last_ping = time_ms();
    }
    //--
}
void ClientNetwork::SetNetworkState(int state,wchar* substatus){
    if(network_substatus != nullptr){free(network_substatus);network_substatus=nullptr;}
    network_substatus = substatus;
    network_state = state;
}
void ClientNetwork::Send(Payload dat){
    server_target.Send(dat);
}
void ClientNetwork::Send(Packet* dat){
    server_target.Send(dat);
}

