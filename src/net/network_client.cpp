#include "network.h"
#include "os_net.h"
#include "packet_builder.h"

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"

#include "../game.h"

using namespace Network;

void ClientSignalConnect(){

}
void ClientSignalDisconnect(wchar* reason){

}

void ClientNetworkThreadEntryPoint(void* network_ptr){
    ClientNetwork* net=(ClientNetwork*)network_ptr;

    Packet transfer_packet;
    bool cleanup=false;
    long loop_start;
    long loop_delta;
    
    while(net->running){
        loop_start=time_ms();
        if(net->server_target.address.IsLocalhost()){
            if(Game::server != nullptr && Game::server->network.running){
                while(net->server_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    Game::server->network.local_client_target.inbound_buffer.Write((byte*)&transfer_packet);
                }
                while(Game::server->network.local_client_target.outbound_buffer.Read((byte*)&transfer_packet)){
                    net->server_target.inbound_buffer.Write((byte*)&transfer_packet);
                    net->server_target.last_recv = time_ms();
                }
            }
            else{cleanup=true;}
        }
        else if(net->server_target.IsConnected()){
           while(net->server_target.outbound_buffer.Read((byte*)&transfer_packet)){
                OSNetwork::send_packet(&transfer_packet,&net->server_target);
            }
            while(OSNetwork::recv_packet(&transfer_packet,&net->server_target)){
                net->server_target.inbound_buffer.Write((byte*)&transfer_packet);
                net->server_target.last_recv = time_ms();
                if(net->server_target.state_id != CONNECTED){
                    net->server_target.SetState(CONNECTED,nullptr);
                    ClientSignalConnect();
                }
            }
        }
        if(net->server_target.state_id < 0 || net->server_target.state_id == DISCONNECTING){
            ClientSignalDisconnect(net->server_target.state_msg);
            cleanup=true;
            continue;//loop one more time to allow disconnect msg to send
        }

        if(cleanup){
            OSNetwork::disconnect(&net->server_target);
            net->server_target.Clear();
            net->running=false;
        }
        loop_delta = time_ms()-loop_start;
        if(loop_delta > 10)sleep_thread(10-loop_delta);
    }
}

ClientNetwork::ClientNetwork():server_target(){
    running=false;
    last_ping=0;
}
ClientNetwork::~ClientNetwork(){
    running=false;
    last_ping=0;
}

void ClientNetwork::Update(){
    if(!server_target.IsConnected())return;
    server_target.Update();
    
    //Ping initiation
    if(time_ms() - last_ping > config::network_ping_interval){
        Packet ping = BuildPacket_PING(time_ms(),0,0);
        Send(&ping);
        last_ping = time_ms();
    }
}
void ClientNetwork::Send(Payload dat){
    server_target.Send(dat);
}
void ClientNetwork::Send(Packet* dat){
    server_target.Send(dat);
}
Payload ClientNetwork::Recieve(){
    return server_target.Recieve();
}


void ClientNetwork::StartConnect(wchar* host_string,Packet* JOIN){
    running=true;
    server_target.SetAddress(host_string);
    server_target.AddToReliableBuffer(JOIN);
    server_target.SetState(CONNECTING,wstr::allocf(L"Connecting to %s ...",host_string));
    OSNetwork::connect(JOIN,&server_target);
    start_thread(ClientNetworkThreadEntryPoint,this);
    last_ping=time_ms();
}
void ClientNetwork::LocalConnect(Packet* JOIN){
    running=true;
    server_target.ForLocal();
    server_target.AddToReliableBuffer(JOIN);
    server_target.SetState(CONNECTING,wstr::new_copy(L"Connecting to local server..."));
    OSNetwork::connect(JOIN,&server_target);
    start_thread(ClientNetworkThreadEntryPoint,this);
    last_ping=time_ms();
}
void ClientNetwork::Disconnect(wchar* reason){ 
    server_target.Disconnect(reason);
}