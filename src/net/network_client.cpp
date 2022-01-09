#include "network.h"
#include "os_net.h"
#include "packet_builder.h"

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"

#include "../client/client_signal.h"

using namespace ClientNetwork;

bool ClientNetwork::running=false;
long ClientNetwork::last_ping=0;
NetTarget ClientNetwork::server_target=NetTarget();

void ClientSignalDisconnect(int code,wchar* reason){
    EventSignal signal;
        signal.type= ClientSignalID::DISCONNECTED_FROM_SERVER;
        signal.params[0].ival=code;
        signal.params[1].strval=reason;
        signal.params[2].ival=0;
    Client::Signal(signal);
}

void ClientNetworkThreadEntryPoint(){
    Packet transfer_packet;
    bool cleanup=false;
    long loop_start;
    long loop_delta;
    
    while(running){
        loop_start=time_ms();
        if(server_target.address.IsLocalhost()){
            if(ServerNetwork::IsRunning()){
                NetTarget* local_server_target= ServerNetwork::GetNetTargetForLocal();
                 while(local_server_target->outbound_buffer.Read((byte*)&transfer_packet)){
                    server_target.inbound_buffer.Write((byte*)&transfer_packet);
                    server_target.last_recv = time_ms();
                    if(server_target.state_id == NetTargetState::ID::CONNECTING){
                        server_target.SetState(NetTargetState::ID::CONNECTED_LOCAL,nullptr);
                    }
                }
            }
            else{cleanup=true;}
            if(server_target.state_id==NetTargetState::DISCONNECTING){cleanup=true;}
        }
        else if(server_target.IsConnected()){
           while(server_target.outbound_buffer.Read((byte*)&transfer_packet)){
                OSNetwork::send_packet(&transfer_packet,&server_target);
            }
            while(OSNetwork::recv_packet(&transfer_packet,&server_target)){
                server_target.inbound_buffer.Write((byte*)&transfer_packet);
                server_target.last_recv = time_ms();
                if(server_target.state_id == NetTargetState::ID::CONNECTING){
                    server_target.SetState(NetTargetState::ID::CONNECTED,nullptr);
                }
            }
        }
        if(server_target.state_id < 0 || server_target.state_id == NetTargetState::ID::DISCONNECTING){
            if(!cleanup){
                ClientSignalDisconnect(server_target.state_id, wstr::new_copy(server_target.state_msg));
                cleanup=true;
                continue;//loop one more time to allow disconnect msg to send
            }
        }
        if(cleanup){
            OSNetwork::disconnect(&server_target);
            server_target.Clear();
            running=false;
        }
        loop_delta = time_ms()-loop_start;
        if(loop_delta < 10 && loop_delta >= 0)sleep_thread(10-loop_delta);
    }
}

void ClientNetwork::Init(){
    running=false;
    last_ping=0;
}
void ClientNetwork::Free(){
    server_target.Disconnect(wstr::new_copy(L"Disconnect by user."));
    last_ping=0;
    while(running);//join with recv thread
}
bool ClientNetwork::IsRunning(){return running;}
NetTarget* ClientNetwork::GetNetTargetForLocal(){return &server_target;}
void ClientNetwork::Update(){
    if(!server_target.IsConnected())return;
    server_target.Update();
    
    //Ping initiation
    if(time_ms() - last_ping > config::network_ping_interval){
        Packet ping_packet;
            PacketData::PING ping_data(&ping_packet);
            ping_data.SetTimestamps(time_ms(),0,0);
            ping_data.WritePacket();
        Send(&ping_packet);
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
    server_target.SetState(NetTargetState::ID::CONNECTING,wstr::allocf(L"Connecting to %ls ...",host_string));
    OSNetwork::connect(JOIN,&server_target);
    start_thread(ClientNetworkThreadEntryPoint);
    last_ping=time_ms();
}
void ClientNetwork::LocalConnect(Packet* JOIN){
    running=true;
    server_target.ForLocal();
    server_target.AddToReliableBuffer(JOIN);
    server_target.SetState(NetTargetState::ID::CONNECTING,wstr::new_copy(L"Connecting to local server..."));
    OSNetwork::connect(JOIN,&server_target);
    start_thread(ClientNetworkThreadEntryPoint);
    last_ping=time_ms();
}
void ClientNetwork::Disconnect(wchar* reason){ 
    server_target.Disconnect(reason);
}