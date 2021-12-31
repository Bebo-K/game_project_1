#include "net_target.h"
#include "packet_builder.h"
#include "os_net.h"
#include "../config.h"

NetTarget::NetTarget():
inbound_buffer(2,MAX_UDP_PACKET_SIZE),
outbound_buffer(2,MAX_UDP_PACKET_SIZE),
reliable_buffer(2),
multipart_buffer(2)
{
    hostname=nullptr;
    address = {0};
    socket=INVALID_SOCKET;
    state_id=NetTarget::NOT_CONNECTED;
    state_msg=nullptr;
    last_recv=0;
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
    state_id=NetTarget::NOT_CONNECTED;
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
    state_id = CONNECTED_LOCAL;
    address = {0};
    address.SetLocalhost(true);
    address.SetValid(true);
}
void NetTarget::Clear(){
    if(hostname != nullptr){free(hostname);hostname=nullptr;}
    if(state_msg != nullptr){free(state_msg);state_msg=nullptr;}
    address= {0};
    socket=INVALID_SOCKET;
    state_id=NetTarget::NOT_CONNECTED;
    last_recv=0;
    conn_start=0;
    Packet msg_dump;
    while(inbound_buffer.Read((byte*)&msg_dump));
    while(outbound_buffer.Read((byte*)&msg_dump));
    reliable_buffer.Clear();
    for(MultipartPayload* payload: multipart_buffer){
        payload->Clear();
    }
}

bool NetTarget::IsConnected(){
    if(state_id == NetTarget::CONNECTED_LOCAL){return true;}
    if(state_id == NetTarget::CONNECTED){
        if(last_recv - time_ms() < config::network_timeout){
            return true;
        }
        else{
            state_id = NetTarget::TIMED_OUT;
            SetStateMsg(wstr::allocf(L"Host %s timed out due to no data recieved",hostname));
        }
    }
    if(state_id == NetTarget::DISCONNECTING){return true;}
    return false;
}
void NetTarget::SetStateMsg(wchar* msg){
    if(state_msg !=nullptr){free(state_msg);state_msg=nullptr;}
    state_msg=msg;
}

void NetTarget::Update(){
    if(IsConnected() || state_id == CONNECTING){
        //Timeout check
        if(IsConnected()){
            if(time_ms() - last_recv > config::network_timeout){
                SetStateMsg(wstr::allocf(L"Lost connection: remote host timed out"));
                state_id = DISCONNECTING;
            }
        }
        else if(state_id == CONNECTING){
            if(time_ms() - conn_start > config::network_timeout){
                SetStateMsg(wstr::allocf(L"Failed to connect: remote host timed out"));
                state_id = DISCONNECTING;
            }
        }//--

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
    else{
        switch(state_id){
            case NETWORK_UNAVAILABLE:   state_id = DISCONNECTING;;break;
            case INVALID:               state_id = DISCONNECTING;;break;
            case UNKNOWN_ERROR:         state_id = DISCONNECTING;;break;
            case DISCONNECTING:break;
            case NOT_CONNECTED:break;
            default:break;
        }
    }
}

void NetTarget::Disconnect(wchar* reason){
    Packet NOPE = BuildPacket_NOPE(reason);
    Send(&NOPE);
    state_id = NetTarget::DISCONNECTING;
    SetStateMsg(reason);
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
        if(packet.id == PacketID::NOPE){
            state_id = NetTarget::DISCONNECTING;
            SetStateMsg(wstr::new_copy(ReadPacket_NOPE_reason(&packet)));
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
            break;
        }
    }
}