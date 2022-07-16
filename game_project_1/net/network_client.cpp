#include <game_project_1/net/network.hpp>
#include <game_project_1/net/os_net.hpp>
#include <game_project_1/net/packets.hpp>

#include <game_project_1/types/data_types.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/os.hpp>

#include <game_project_1/client/client_signal.hpp>

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
    bool cleanup_loop=true;
    long loop_start;
    long loop_delta;
    Datagram packet_buffer;
    
    while(running||cleanup_loop){   if(!running){cleanup_loop=false;}
        loop_start=OS::time_ms();

        if(server_target.IsConnected()){
            while(server_target.outbound_buffer.Read((byte*)&packet_buffer)){
                server_target.connection.Write(&packet_buffer);
            }
            while(server_target.connection.Read(&packet_buffer)){
                server_target.inbound_buffer.Write((byte*)&packet_buffer);
                server_target.last_success = OS::time_ms();
            }
        }
        else if(server_target.IsJustDisconnected()){
            ClientSignalDisconnect(server_target.connection.state, wstr::new_copy(server_target.connection.message));
            server_target.connection.SetState(0,null);
            running=false;
            continue;
        }
        loop_delta = OS::time_ms()-loop_start;
        if(loop_delta < 10 && loop_delta >= 0)OS::SleepThread(10-loop_delta);
    }
    server_target.Clear();
}

void ClientNetwork::Init(){
    running=false;
    last_ping=0;
}

void ClientNetwork::Free(){
    if(server_target.connection.IsConnected()){
        server_target.SendDisconnect(L"Disconnect by user.");
    }
    last_ping=0;
    int join_timeout = 5000;
    while(running && join_timeout > 0){join_timeout--;OS::SleepThread(1);}//join with recv thread
    logger::info("Disconnected from server.");
    //TODO: join timeout error msg
}

void ClientNetwork::Update(){
    if(!server_target.IsConnected())return;
    server_target.Update();
    
    //Ping initiation
    if(OS::time_ms() - last_ping > config::network_ping_interval){
        Packet::PING ping_packet;
            ping_packet.ts_1=OS::time_ms();

        server_target.Send(ping_packet.GetPayload());
        last_ping = OS::time_ms();
    }
}

void ClientNetwork::Send(Payload dat){
    server_target.Send(dat);
}

Payload ClientNetwork::Recieve(){
    return server_target.Recieve();
}

void ClientNetwork::StartConnect(wchar* host_string,wchar* persona, int save_id){
    Packet::JOIN JOIN;
        JOIN.player_save_id=save_id;
        JOIN.SetPersona(persona);
    server_target.ForAddress(host_string);
    server_target.SendConnect(JOIN.GetPayload());
    running=true;
    OS::StartThread(ClientNetworkThreadEntryPoint);
    last_ping=OS::time_ms();
}

void ClientNetwork::LocalConnect(wchar* persona, int save_id){
    Packet::JOIN JOIN;
        JOIN.player_save_id=save_id;
        JOIN.SetPersona(persona);
    server_target.ForLocal(&ServerNetwork::GetNetTargetForLocal()->inbound_buffer);
    server_target.SendConnect(JOIN.GetPayload());
    running=true;
    OS::StartThread(ClientNetworkThreadEntryPoint);
    last_ping=OS::time_ms();
}

void ClientNetwork::Disconnect(wchar* reason){ 
    server_target.SendDisconnect(reason);
}

bool ClientNetwork::IsRunning(){return running;}
NetTarget* ClientNetwork::GetNetTargetForLocal(){return &server_target;}