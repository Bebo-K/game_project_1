#include <game_project_1/net/net_target.hpp>
#include <game_project_1/net/packet_builder.hpp>
#include <game_project_1/net/os_net.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/os.hpp>

using namespace NetTargetState;

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
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){ackd_reliables[i]=0;}
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){sent_reliables[i]=0;}
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
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){ackd_reliables[i]=0;}
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){sent_reliables[i]=0;}
}

bool NetTarget::IsConnected(){return (state_id > 0);}

void NetTarget::SetState(NetTargetState::ID state,wchar* msg){
    if(state_id != state){
        state_id = state;//run state change callback
    }
    if(state_msg !=nullptr){free(state_msg);state_msg=nullptr;}
    state_msg=msg;
}

void NetTarget::Update(){
    if(IsConnected()){
        //Timeout check
        if(state_id == CONNECTING && OS::time_ms() - conn_start > config::network_timeout){
            SetState(TIMED_OUT,wstr::new_copy(L"Failed to connect: remote host timed out"));
        }
        if((state_id == CONNECTED || state_id == CONNECTED_LOCAL) && OS::time_ms() - last_recv > config::network_timeout){
            SetState(TIMED_OUT,wstr::new_copy(L"Lost connection: remote host timed out"));
        }

        //Reliable resends
        for(ReliablePayloadEnvelope *reliable: reliable_buffer){
            if(reliable->ShouldSend()){
                if(reliable->retry_count > config::network_resend_max){
                    logger::warn("Reliable payload ID %d exceeded retry count of %d, dropping!",reliable->reliable_id,config::network_resend_max);
                    RemoveFromReliableBuffer(reliable->reliable_id);
                }
                else{
                    Payload reliable_payload;
                    reliable_payload.id = reliable->reliable_id;
                    reliable_payload.type = reliable->type;
                    reliable_payload.length = reliable->length;
                    reliable_payload.data = reliable->data;
                    Send(reliable_payload);
                    reliable->last_sent= OS::time_ms();
                    reliable->retry_count++;
                }
            }
        }//--
    }
}

void NetTarget::Disconnect(wchar* reason){
    Packet NOPE;
    PacketData::NOPE disconnect_data(&NOPE);
        disconnect_data.SetReason(reason);
        disconnect_data.WritePacket();
    Send(&NOPE);
    SetState(DISCONNECTING,reason);
}


void NetTarget::Send(Packet* packet){
    packet->timestamp=OS::time_ms();
    packet->crc = CRC((byte*)packet,packet->length);
    outbound_buffer.Write((byte*)packet);
}
void NetTarget::Send(Payload payload){
    if(payload.length > Packet::MAX_DATA_LENGTH){
        MultipartPacket packet;
        packet.type = payload.type;
        packet.timestamp=OS::time_ms();
        if(payload.id == 0){packet.CreateID();}
        else{packet.id = payload.id;}
        
        packet.segment_count=(payload.length/MultipartPacket::MAX_DATA_LENGTH);
            if(payload.length%MultipartPacket::MAX_DATA_LENGTH != 0){packet.segment_count +=1;}
        packet.payload_offset=0;
        packet.SetPayloadLength(payload.length);

        int data_length;
        for(int i=0; i<packet.segment_count; i++){
            data_length = MultipartPacket::MAX_DATA_LENGTH;
            if(packet.payload_offset + data_length > payload.length){
                data_length = payload.length - packet.payload_offset;
            }
            packet.ClearData();
            packet.segment=i;
            packet.SetPacketDataLength(data_length);
            memcpy(&packet.data,&payload.data[packet.payload_offset],data_length);
            packet.crc = CRC((byte*)&packet,MultipartPacket::HEADER_SIZE + packet.packet_data_length);
            outbound_buffer.Write((byte*)&packet);
            packet.payload_offset+=data_length;
        }
    }
    else{
        Packet packet;
        packet.type = payload.type;
        if(payload.id == 0){packet.CreateID();}
        else{packet.id = payload.id;}
        packet.ClearData();
        packet.timestamp=OS::time_ms();
        packet.SetDataLength(payload.length);
        memcpy(&packet.data,payload.data,packet.length);
        packet.crc = CRC((byte*)&packet,packet.length);
        outbound_buffer.Write((byte*)&packet);
    }
}
Payload NetTarget::Recieve(){
    Packet packet;
    while(inbound_buffer.Read((byte*)&packet)){
        if(packet.IsMultipart()){
            MultipartPayload* payload = AddToMultipartPayload((MultipartPacket*)&packet);
            if(payload->IsFullyAssembled()){
                Payload finished_payload = FinishMultipartPayload(payload->id);
                if(finished_payload.IsReliable()){
                    bool duplicate = !Acknowledge(finished_payload.id);
                    if(duplicate){
                        if(finished_payload.free_after_use){free(finished_payload.data);}
                        continue;
                    }
                    else return finished_payload;
                }
            }
            else{continue;}
        }
        if(packet.IsReliable()){
            if(!Acknowledge(packet.id)){
                continue;
            }
        }
        if(packet.type == PacketID::OKAY){
            PacketData::OKAY ack_data(&packet);
            RemoveFromReliableBuffer(ack_data.GetAckID());
            continue;
        }
        if(packet.type == PacketID::ACPT){
            PacketData::ACPT accept_data(&packet);
            RemoveFromReliableBuffer(accept_data.GetAckID());
        }
        if(packet.type == PacketID::NOPE){
            PacketData::NOPE disconnect_data(&packet);
            SetState(DISCONNECTING,wstr::new_copy(disconnect_data.GetReason()));
        }
        if(packet.type == PacketID::PING){
            PacketData::PING ping_data(&packet);

            if(ping_data.GetTimestamp2() == 0){
                Packet response_ping;
                PacketData::PING response_ping_data(&response_ping);
                response_ping_data.SetTimestamps(ping_data.GetTimestamp1(),OS::time_ms(),0);
                response_ping_data.WritePacket();
                Send(&response_ping);
            }
            else if(ping_data.GetTimestamp3() == 0){
                latency= OS::time_ms() - ping_data.GetTimestamp1();
                Packet response_ping;
                PacketData::PING response_ping_data(&response_ping);
                response_ping_data.SetTimestamps(ping_data.GetTimestamp1(),ping_data.GetTimestamp2(),OS::time_ms());
                response_ping_data.WritePacket();
                Send(&response_ping);
            }
            else{
                latency= OS::time_ms() - ping_data.GetTimestamp2();
            }
            continue;
        }

        memcpy(&buffer_packet,&packet,sizeof(Packet));
        return Payload(buffer_packet);
    }
    return Payload(0,0,0,nullptr);
}

bool NetTarget::Acknowledge(int ack_id){//always send back an OKAY packet, returns true if not acknowledged before
    bool duplicate=false;
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){
        if(ackd_reliables[i]==ack_id){
            duplicate=true;
        }
    }
    if(!duplicate){//add to ack buffer
        for(int i=RELIABLE_ID_HISTORY-1;i>0;i--){ackd_reliables[i] = ackd_reliables[i-1];} 
        ackd_reliables[0] = ack_id;
    }
    Packet ack;PacketData::OKAY ack_data(&ack);
        ack_data.SetAckID(ack_id);
        ack_data.WritePacket();
        Send(&ack);
    return !duplicate;
}

int NetTarget::NewReliableID(){
    int reliable_id = rand();
    for(int i=0;i<RELIABLE_ID_HISTORY;i++){
        if(reliable_id == sent_reliables[i] || reliable_id==0){i=0;reliable_id = rand();}
    }
    for(int i=RELIABLE_ID_HISTORY-1;i>0;i--){sent_reliables[i] = sent_reliables[i-1];}
    sent_reliables[0]=reliable_id;
    return reliable_id;
}

void NetTarget::AddToReliableBuffer(Payload payload){
    ReliablePayloadEnvelope* reliable = reliable_buffer.Add();
    reliable->type = payload.type;
    reliable->length = payload.length;
    reliable->data = (byte*)calloc(payload.length,1);
    memcpy(reliable->data,payload.data,payload.length);


    reliable->reliable_id = NewReliableID();
    reliable->last_sent=0;
    reliable->retry_count=0;
}

void NetTarget::RemoveFromReliableBuffer(int reliable_id){
    for(ReliablePayloadEnvelope* reliable: reliable_buffer){
        if(reliable->reliable_id == reliable_id){
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
    new_payload->Add(packet);
    return new_payload;
}
void NetTarget::RemoveFromMultipartBuffer(int payload_id){
    for(MultipartPayload* payload: multipart_buffer){
        if(payload->id == payload_id){
            multipart_buffer.Delete(multipart_buffer.IndexOf(payload));
            break;
        }
    }
}
Payload NetTarget::FinishMultipartPayload(int payload_id){
    for(MultipartPayload* payload: multipart_buffer){
        if(payload->id == payload_id){
            Payload ret(payload->id,payload->type,payload->len,
                payload->last_recv,payload->assembled_payload);
                
            payload->assembled_payload=nullptr;//ret now owns pointer;
            multipart_buffer.Delete(multipart_buffer.IndexOf(payload));
            ret.free_after_use=true;
            return ret;
        }
    }
    return Payload(0,0,0,nullptr);
}