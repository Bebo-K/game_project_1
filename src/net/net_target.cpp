#include "net_target.h"
#include "packet_builder.h"
#include "os_net.h"
#include "../config.h"

using namespace Network;
NetTarget::NetTarget():
reliable_buffer(2),
multipart_buffer(2),
inbound_buffer(MAX_UDP_PACKET_SIZE,2),
outbound_buffer(MAX_UDP_PACKET_SIZE,2)
{
    hostname=nullptr;
    address = {0};
    socket=INVALID_SOCKET;
    state_id=NOT_CONNECTED;
    state_msg=nullptr;
    last_recv=0;
    conn_start=0;
    latency=0;
}
NetTarget::~NetTarget(){//:~reliable_buffer(),~inbound_buffer(),~outbound_buffer()
    for(MultipartPayload* multipart: multipart_buffer){multipart->Clear();}
    if(hostname !=nullptr){
        free(hostname);
        hostname=nullptr;
    }
    if(state_msg !=nullptr){free(state_msg);state_msg=nullptr;}
    address = {0};
    socket=INVALID_SOCKET;
    SetState(NOT_CONNECTED,nullptr);
}

void NetTarget::SetAddress(wchar* hostname,unsigned short port){
    address.port=htons(port);
    hostname = wstr::new_copy(hostname);
    address = OSNetwork::DNS_lookup(hostname,address.port);
}
void NetTarget::SetAddress(wchar* host_and_port){
    wchar* port_string = wstr::substr_after_last(host_and_port,':');
    if(port_string != null){
        address.port=htons((unsigned short)wstr::integer_from_string(port_string));
        free(port_string);
        hostname = wstr::substr_before_last(hostname,':');
    }
    else{
        address.port=htons(server_config::default_port);
        hostname = wstr::new_copy(host_and_port);
    }

    address = OSNetwork::DNS_lookup(hostname,address.port);
}
void NetTarget::SetAddress(ip_address address){
    hostname = nullptr;
    address = address;
}
void NetTarget::ForLocal(){
    SetState(CONNECTED_LOCAL,nullptr);
    address = {0};
    address.SetLocalhost(true);
    address.SetValid(true);
    latency=0;
}
void NetTarget::Clear(){
    if(hostname != nullptr){free(hostname);hostname=nullptr;}
    if(state_msg != nullptr){free(state_msg);state_msg=nullptr;}
    SetState(NOT_CONNECTED,nullptr);
    address= {0};
    socket=INVALID_SOCKET;
    last_recv=0;
    conn_start=0;
    latency=0;
    Packet msg_dump;
    while(inbound_buffer.Read((byte*)&msg_dump));
    while(outbound_buffer.Read((byte*)&msg_dump));
    reliable_buffer.Clear();
    for(MultipartPayload* payload: multipart_buffer){
        payload->Clear();
    }
    memset(&buffer_packet,0,sizeof(Packet));
}

bool NetTarget::IsConnected(){return (state_id > 0);}

void NetTarget::SetState(NetTargetState state,wchar* msg){
    if(state_id != state){
        state_id = state;
        //run state change callback
    }
    if(state_msg !=nullptr){free(state_msg);state_msg=nullptr;}
    state_msg=msg;
}

void NetTarget::Update(){
    if(IsConnected()){
        //Timeout check
        if(state_id == CONNECTING && time_ms() - conn_start > config::network_timeout){
            SetState(TIMED_OUT,wstr::allocf(L"Failed to connect: remote host timed out"));
        }
        if(state_id == CONNECTED && time_ms() - last_recv > config::network_timeout){
            SetState(TIMED_OUT,wstr::allocf(L"Lost connection: remote host timed out"));
        }

        //Reliable resends
        for(ReliablePacketEnvelope *reliable: reliable_buffer){
            if(reliable->ShouldSend()){
                if(reliable->retry_count > config::network_resend_max){
                    logger::warn("Reliable packet ID %d exceeded retry count of %d, dropping!",reliable->dataPacket.id,config::network_resend_max);
                    RemoveFromReliableBuffer(reliable->dataPacket.id);
                }
                else{
                    Send(&reliable->dataPacket);
                    reliable->last_sent= time_ms();
                    reliable->retry_count++;
                }
            }
        }//--
    }
}

void NetTarget::Disconnect(wchar* reason){
    Packet NOPE = BuildPacket_NOPE(reason);
    Send(&NOPE);
    SetState(DISCONNECTING,reason);
}

void NetTarget::Send(Packet* packet){
    outbound_buffer.Write((byte*)packet);
}
void NetTarget::Send(Payload payload){
    if(payload.length > Packet::MAX_DATA_LENGTH){
        MultipartPacket packet;
        packet.length=MultipartPacket::MAX_DATA_LENGTH;
        packet.type = payload.type;
        packet.CreateID();
        
        packet.segment_count=(payload.length/MultipartPacket::MAX_DATA_LENGTH);
        packet.payload_offset=0;
        packet.payload_length= payload.length;

        for(packet.segment=0; packet.segment<packet.segment_count; packet.segment++){
            if(packet.payload_offset + packet.length > payload.length){
                packet.length= payload.length - packet.payload_offset;
            }
            packet.ClearData();
            memcpy(&packet.data,&payload.data[packet.payload_offset],packet.length);
            packet.RunCRC();
            outbound_buffer.Write((byte*)&packet);
            packet.payload_offset+=packet.length;
        }
    }
    else{
        Packet packet;
        packet.type = payload.type;
        packet.CreateID();
        packet.ClearData();
        packet.length = payload.length;
        memcpy(&packet.data,payload.data,packet.length);
        packet.RunCRC();
        outbound_buffer.Write((byte*)&packet);
    }
}
Payload NetTarget::Recieve(){
    Packet packet;
    while(inbound_buffer.Read((byte*)&packet)){
        if(packet.IsReliable()){
            Packet ack = BuildPacket_OKAY(packet.id);
            Send(&ack);
        }
        if(packet.id == PacketID::OKAY){
            int reliable = ReadPacket_OKAY_ackID(&packet);
            RemoveFromReliableBuffer(reliable);
            continue;
        }
        if(packet.id == PacketID::ACPT){
            int reliable = ReadPacket_ACPT_ackID(&packet);
            RemoveFromReliableBuffer(reliable);
        }
        if(packet.id == PacketID::NOPE){
            SetState(DISCONNECTING,wstr::new_copy(ReadPacket_NOPE_reason(&packet)));
        }
        if(packet.id == PacketID::PING){
            int ts_1,ts_2,ts_3;
            ts_1 = ReadPacket_PING_timestamp(&packet,0);
            ts_2 = ReadPacket_PING_timestamp(&packet,1);
            ts_3 = ReadPacket_PING_timestamp(&packet,2);

            if(ts_2 == 0){
                Packet ping_2  = BuildPacket_PING(ts_1,time_ms(),0);
                Send(&ping_2);
            }
            else if(ts_3 == 0){
                latency= time_ms() - ts_1;
                Packet ping_3  = BuildPacket_PING(ts_1,ts_2,time_ms());
                Send(&ping_3);
            }
            else{
                latency= time_ms() - ts_2;
            }
            continue;
        }
        if(packet.IsMultipart()){
            MultipartPayload* payload = AddToMultipartPayload((MultipartPacket*)&packet);
            if(payload->IsFullyAssembled()){
                return FinishMultipartPayload(payload->id);
            }
            else{continue;}
        }
        memcpy(&buffer_packet,&packet,sizeof(Packet));
        Payload ret(buffer_packet.type,buffer_packet.length-Packet::HEADER_SIZE,buffer_packet.data);
        return ret;
    }
    return Payload(0,0,nullptr);
}


void NetTarget::AddToReliableBuffer(Packet* packet){
    ReliablePacketEnvelope* reliable = reliable_buffer.Add();
    reliable->last_sent=0;
    reliable->retry_count=0;
    memcpy(&reliable->dataPacket,packet,sizeof(Packet));
}
void NetTarget::RemoveFromReliableBuffer(int packet_id){
    for(ReliablePacketEnvelope* reliable: reliable_buffer){
        if(reliable->dataPacket.id == packet_id){
            reliable_buffer.Delete(reliable_buffer.IndexOf(reliable));
            break;
        }
    }
}

MultipartPayload* NetTarget::AddToMultipartPayload(MultipartPacket* packet){
    for(MultipartPayload* payload: multipart_buffer){
        if(payload->id == packet->id){
            payload->Add(packet);
            return payload;
        }
    }
    MultipartPayload* new_payload = multipart_buffer.Add();
    new_payload->Start(packet);
    return new_payload;
}
void NetTarget::RemoveFromMultipartBuffer(int packet_id){
    for(MultipartPayload* payload: multipart_buffer){
        if(payload->id == packet_id){
            multipart_buffer.Delete(multipart_buffer.IndexOf(payload));
            break;
        }
    }
}
Payload NetTarget::FinishMultipartPayload(int packet_id){
    for(MultipartPayload* payload: multipart_buffer){
        if(payload->id == packet_id){
            Payload ret(payload->type,payload->len,payload->assembled_payload);
            payload->assembled_payload=nullptr;//ret now owns pointer;
            multipart_buffer.Delete(multipart_buffer.IndexOf(payload));
            return ret;
        }
    }
    return Payload(0,0,nullptr);
}