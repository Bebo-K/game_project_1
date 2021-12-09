#include "network.h"
#include <stdlib.h>

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"


void TryConnectNewTarget(char* address,Network* net){
    int new_target_id = -1;
    for(int i=0;i<net->target_count;i++){if(!net->targets[i].connected){new_target_id=i;break;}}
    if(new_target_id >= 0){
        net->targets->SetAddress(address);
        //TODO: this won't do anything
    }
}

void NetworkThreadEntryPoint(void* network_ptr){
    Network* net=(Network*)network_ptr;

    int connected_targets=0;
    while(!net->offline){
        connected_targets=0;
        for(int i=0;i<net->target_count;i++){
            if(!net->targets[i].connected){continue;}
            connected_targets++;

            Packet to_send;
            while(net->targets[i].write_buffer.Read((byte*)&to_send)){
                OSNetwork::send(&to_send,net->targets[i].os_socket);
            }
        }

        if(net->listener_enabled){
            if(net->listener_port<0){
                //Startup listener
                net->listener_socket = OSNetwork::bind_to_port(net->listener_port);
            }
            //Server read to inbound buffer
            //if(connected_targets >= net->target_count){
                //refuse further new connections
            //}
        }
    }
}

void Network::Initialize(int max_targets){
    targets = new NetTarget[max_targets];
    target_count = max_targets;
    listener_port=-1;
    listener_socket=0;
    offline = !(OSNetwork::init());

    if(!offline){
        start_thread(NetworkThreadEntryPoint,this);
    }
}
void Network::Destroy(){
    offline=true;
    OSNetwork::destroy();
    delete[] targets;
    target_count=0;
}
void Network::StartListener(short port){

}//enables listener on net thread
int Network::AddTarget(char* address){

}//initializes a new target for network thead. Returns target id(index)
void Network::WriteToTarget(int target_id,Payload payload){
    if(payload.length+20 < MAX_UDP_PACKET_SIZE){
        Packet to_write;
        to_write.id=rand();
        to_write.type=payload.type;
        to_write.length=payload.length+20;
        memcpy(&to_write.data,payload.data,payload.length);
        to_write.runCRC();
        targets[target_id].write_buffer.Write((byte*)&to_write);
    }
    else{
        MultipartPacket to_write;
        to_write.id=rand();
        to_write.type=payload.type;
        to_write.length=payload.length;
        to_write.segment_count=(payload.length / (MAX_UDP_PACKET_SIZE-20))+1;
        to_write.segment_offset=0;

        for(to_write.segment=0; to_write.segment<to_write.segment_count; to_write.segment++){
            to_write.segment_length= (payload.length-to_write.segment_offset > MAX_UDP_PACKET_SIZE-24)? MAX_UDP_PACKET_SIZE-24 : payload.length-to_write.segment_offset;
            memcpy(&to_write.data,payload.data,to_write.length);
            to_write.runCRC();
            targets[target_id].write_buffer.Write((byte*)&to_write);
            to_write.segment_offset += to_write.length;
        }
    }
}

Payload Network::ReadFromTarget(int target_id){
    Packet read_packet;
    while(targets[target_id].read_buffer.Read((byte*)&read_packet)){
        if(read_packet.isMultipart()){
            //TODO:
        }
        else{
            return Payload(read_packet.type,read_packet.length,read_packet.data);
        }
    }
    return Payload(0,0,nullptr);
}


void NetTarget::SetAddress(char* address){
    char* port_string = cstr::substr_after_last(address,':');
    if(port_string != null){
        port=(unsigned short)cstr::integer_from_string(port_string);
        free(port_string);
        hostname = cstr::substr_before_last(hostname,':');
    }
    else{
        port=server_config::default_port;
        hostname = cstr::new_copy(address);
    }
}

NetTarget::NetTarget():read_buffer(2,MAX_UDP_PACKET_SIZE),write_buffer(2,MAX_UDP_PACKET_SIZE),packet_assembly_buffer(2,sizeof(MultipartPayload)){
//TODO:
}
NetTarget::~NetTarget(){
//TODO:
}
