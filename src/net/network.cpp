#include "network.h"
#include <stdlib.h>

#include "../struct/data_types.h"
#include "../config.h"
#include "../os.h"


void NetworkThreadEntryPoint(void* network_ptr){
    Network* net=(Network*)network_ptr;

    while(!net->offline){
        for(int i=0;i<net->target_count;i++){
            if(!net->targets[i].connected){continue;}

            Packet to_send;
            while(net->targets[i].write_buffer.Read((byte*)&to_send)){
                OSNetwork::send_packet(&to_send,net->targets[i].os_socket);
            }
            while(OSNetwork::recv_packet(&net->targets[i].os_socket)){

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

}
int Network::AddTarget(char* address){

}
void Network::WriteToTarget(int target_id,Payload payload){
    if(payload.length+20 < MAX_UDP_PACKET_SIZE){
        Packet to_write;
        to_write.id=rand();
        to_write.type=payload.type;
        to_write.length=payload.length+20;
        memcpy(&to_write.data,payload.data,payload.length);
        to_write.runCRC();
        targets[target_id].write_buffer.Write((byte*)&to_write);
        if(to_write.isReliable()){
            targets[target_id].AddToReliableBuffer(&to_write);
        }
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
            if(/*TODO: should all multipart packets be reliable?*/ true){
                targets[target_id].AddToReliableBuffer((Packet*)&to_write);
            }
            to_write.segment_offset += to_write.length;
        }
    }
}


Payload Network::ReadFromTarget(int target_id){
    Packet read_packet;
    while(targets[target_id].read_buffer.Read((byte*)&read_packet)){
        if(read_packet.isMultipart()){
            MultipartPayload* multipart_payload = targets[target_id].GetMultipartPayload((MultipartPacket*)&read_packet);
            if(multipart_payload->isFullyAssembled()){
                Payload ret = OpenPayload(read_packet.type,read_packet.length,multipart_payload->assembled_payload);
                multipart_payload->Clear();
                return ret;
            }
        }
        else{
            return OpenPayload(read_packet.type,read_packet.length,read_packet.data);
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

NetTarget::NetTarget():read_buffer(2,MAX_UDP_PACKET_SIZE),write_buffer(2,MAX_UDP_PACKET_SIZE){
    hostname=nullptr;
    outbound_buffer=nullptr;
    outbound_buffer_len=0;
    inbound_buffer=nullptr;
    inbound_buffer_len=0;
    port=0;
    os_socket=0;
    connected=false;
    latency=0;
}

NetTarget::~NetTarget(){
    if(hostname != nullptr){free(hostname);hostname=nullptr;}
    OSNetwork::disconnect(os_socket);os_socket=0;
    port=0;
    connected=false;
    latency=0;
}
