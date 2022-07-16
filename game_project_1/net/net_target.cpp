#include <game_project_1/net/net_target.hpp>
#include <game_project_1/net/packets.hpp>
#include <game_project_1/net/os_net.hpp>
#include <game_project_1/config.hpp>
#include <game_project_1/io/crc.hpp>
#include <game_project_1/os.hpp>

NetTarget::NetTarget() : connection(),
                         reliable_buffer(2),
                         multipart_buffer(2),
                         inbound_buffer(MAX_UDP_PACKET_SIZE, 2),
                         outbound_buffer(MAX_UDP_PACKET_SIZE, 2)
{
    last_success = 0;
    latency = 0;
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        ackd_reliables[i] = 0;
    }
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        sent_reliables[i] = 0;
    }
}
NetTarget::~NetTarget()
{ //:~connection(),~reliable_buffer(),~inbound_buffer(),~outbound_buffer()
    last_success = 0;
    latency = 0;
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        sent_reliables[i] = 0;
        ackd_reliables[i] = 0;
    }
    for (InboundMultipartPayload *multipart : multipart_buffer)
    {
        multipart->Clear();
    }
}

void NetTarget::ForLocal(SynchronousBuffer *buffer_link)
{
    connection.LocalLink(buffer_link);
    last_success = OS::time_ms();
}
void NetTarget::ForAddress(wchar *host, unsigned short port)
{
    connection.FromHostPort(host, port);
}
void NetTarget::ForAddress(wchar *host_and_port)
{
    wchar *port_string = wstr::substr_after_last(host_and_port, ':');
    if (port_string != null)
    {
        wchar *host_string = wstr::substr_before_last(host_and_port, ':');
        connection.FromHostPort(host_string, wstr::integer_from_string(port_string));
        free(host_string);
        free(port_string);
    }
    else
    {
        connection.FromHostPort(host_and_port, server_config::default_port);
    }
}
void NetTarget::ForAddress(NetAddress address)
{
    connection.FromIPAddress(address);
}

void NetTarget::Clear()
{
    if (connection.IsConnected())
    {
        connection.Disconnect();
    }
    connection.Clear();
    last_success = 0;
    latency = 0;

    Datagram msg_dump;
    while (inbound_buffer.Read((byte *)&msg_dump))
        ;
    while (outbound_buffer.Read((byte *)&msg_dump))
        ;
    for (InboundMultipartPayload *payload : multipart_buffer)
    {
        payload->Clear();
    }
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        ackd_reliables[i] = 0;
    }
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        sent_reliables[i] = 0;
    }
    reliable_buffer.Clear();
    multipart_buffer.Clear();
}

void NetTarget::Update()
{
    if (!connection.IsConnected())
    {
        return;
    }
    // Timeout checks
    if (connection.state == Connection::CONNECTING && OS::time_ms() - last_success > config::network_timeout)
    {
        connection.SetState(Connection::DISCONNECTED, wstr::new_copy(L"Failed to connect: remote host timed out"));
        return;
    }
    if (connection.state == Connection::CONNECTED && OS::time_ms() - last_success > config::network_timeout)
    {
        connection.SetState(Connection::DISCONNECTED, wstr::new_copy(L"Lost connection: remote host timed out"));
        return;
    }

    // Reliable resends
    for (OutboundReliablePayload *reliable : reliable_buffer)
    {
        if (reliable->ShouldSend())
        {
            if (reliable->retry_count > config::network_resend_max)
            {
                logger::warn("Reliable payload ID %d exceeded retry count of %d, dropping!", reliable->id, config::network_resend_max);
                RemoveFromReliableBuffer(reliable->id);
            }
            else
            {
                Transfer(reliable->GetPayload());
                reliable->last_sent = OS::time_ms();
                reliable->retry_count++;
            }
        }
    }
}

bool NetTarget::IsConnected()
{
    return connection.IsConnected();
}

bool NetTarget::IsJustDisconnected()
{
    return connection.state < 0;
}

bool NetTarget::IsLocal()
{
    return connection.address.IsLocalhost();
}

void NetTarget::Send(Payload payload)
{  
    if (payload.IsReliable()){   
        payload.id = NewReliablePayloadID();
        AddToReliableBuffer(payload);
    }
    else{
        payload.id = rand();
    }
    Transfer(payload);
}

void NetTarget::Transfer(Payload payload)
{
    Datagram datagram;
    if (payload.length > Datagram::MAX_DATA_LENGTH)
    {
        int max_segments = datagram.FromPayload(payload, 0);
        outbound_buffer.Write((byte *)&datagram);
        for (int i = 1; i < max_segments; i++)
        {
            datagram.FromPayload(payload, i);
            outbound_buffer.Write((byte *)&datagram);
        }
    }
    else
    {
        datagram.FromPayload(payload);
        outbound_buffer.Write((byte *)&datagram);
    }
    if (payload.type == PacketID::NOPE)
    {
        connection.SetState(Connection::DISCONNECTED, wstr::new_copy(Packet::NOPE(payload).reason_buffer));
    }
}

// Returns true if the payload should be forwarded to the net target, or false if we handle it here
bool NetTarget::OnRecieve(Payload payload)
{
    if (payload.IsReliable())
    {
        if (!SendAck(payload.id))
        {
            return false;
        } // If already acknowleged don't forward
    }
    if (payload.type == PacketID::OKAY)
    { // Acknowledgement- don't forward
        RemoveFromReliableBuffer(Packet::OKAY(payload).ack_id);
        return false;
    }
    if (payload.type == PacketID::ACPT)
    { // Special ack from server accept request- Still forward
        RemoveFromReliableBuffer(Packet::ACPT(payload).ack_id);
        return true;
    }
    if (payload.type == PacketID::NOPE)
    { // Disconnect- Still forward so target can handle it as well.
        connection.SetState(Connection::DISCONNECTED, wstr::new_copy(Packet::NOPE(payload).reason_buffer));
        return true;
    }
    if (payload.type == PacketID::PING)
    { // Ping- We just use these for measuring latency and keeping alive connections. Don't forward.
        Packet::PING ping_data(payload);

        if (ping_data.ts_2 == 0)
        {
            ping_data.ts_2 = OS::time_ms();
            Send(ping_data.GetPayload());
        }
        else if (ping_data.ts_3 == 0)
        {
            latency = OS::time_ms() - ping_data.ts_1;
            ping_data.ts_3 = OS::time_ms();
            Send(ping_data.GetPayload());
        }
        else
        {
            latency = OS::time_ms() - ping_data.ts_2;
        }
        return false;
    }
    return true;
}

Payload NetTarget::Recieve()
{
    Datagram datagram;
    while (inbound_buffer.Read((byte *)&datagram))
    {
        if (IsLocal())
        {
            last_success = OS::time_ms();
        } // Normally this is set on connection.
        if (datagram.IsMultipart())
        {
            InboundMultipartPayload *mp_payload = AddToMultipartPayload(&datagram);
            if (mp_payload->IsFullyAssembled())
            {
                Payload finished_payload = mp_payload->Finish();
                multipart_buffer.Delete(multipart_buffer.IndexOf(mp_payload));
                if (OnRecieve(finished_payload))
                {
                    return finished_payload;
                }
            }
        }
        else
        {
            Payload payload = datagram.ToPayload();
            if (OnRecieve(payload))
            {
                return payload;
            }
        }
    }
    return Payload(0, 0, 0, 0, nullptr);
}

void NetTarget::SendConnect(Payload join_payload)
{   
    join_payload.id = NewReliablePayloadID();
    Datagram join_request;
    join_request.FromPayload(join_payload);
    connection.Connect(&join_request);

    OutboundReliablePayload *join_reliable = reliable_buffer.Add();
    join_reliable->id = join_payload.id;
    join_reliable->type = join_payload.type;
    join_reliable->length = join_payload.length;
    join_reliable->data = (byte *)calloc(join_payload.length, 1);
    memcpy(join_reliable->data, join_payload.data, join_payload.length);

    join_reliable->last_sent = OS::time_ms();
    join_reliable->retry_count = 1;
}

bool NetTarget::SendAck(int ack_id)
{ // always send back an OKAY packet, returns true if not acknowledged before
    bool duplicate = false;
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        if (ackd_reliables[i] == ack_id)
        {
            duplicate = true;
        }
    }
    if (!duplicate)
    { // add to ack buffer and push it back
        for (int i = RELIABLE_ID_HISTORY - 1; i > 0; i--)
        {
            ackd_reliables[i] = ackd_reliables[i - 1];
        }
        ackd_reliables[0] = ack_id;
    }
    Packet::OKAY ack;
    ack.ack_id = ack_id;
    Send(ack.GetPayload());
    return !duplicate;
}

void NetTarget::SendDisconnect(wchar *reason)
{
    Send(Packet::NOPE(reason).GetPayload());
}

int NetTarget::NewReliablePayloadID()
{
    int reliable_id = rand();
    for (int i = 0; i < RELIABLE_ID_HISTORY; i++)
    {
        if (reliable_id == sent_reliables[i] || reliable_id == 0)
        {
            i = 0;
            reliable_id = rand();
        }
    }
    for (int i = RELIABLE_ID_HISTORY - 1; i > 0; i--)
    {
        sent_reliables[i] = sent_reliables[i - 1];
    }
    sent_reliables[0] = reliable_id;
    return reliable_id;
}

int NetTarget::AddToReliableBuffer(Payload payload)
{
    OutboundReliablePayload *reliable = reliable_buffer.Add();
    reliable->id = payload.id;
    reliable->type = payload.type;
    reliable->length = payload.length;
    reliable->data = (byte *)calloc(payload.length, 1);
    memcpy(reliable->data, payload.data, payload.length);

    reliable->last_sent = 0;
    reliable->retry_count = 0;
    return reliable->id;
}

void NetTarget::RemoveFromReliableBuffer(int reliable_id)
{
    for (OutboundReliablePayload *reliable : reliable_buffer)
    {
        if (reliable->id == reliable_id)
        {
            reliable_buffer.Delete(reliable_buffer.IndexOf(reliable));
            break;
        }
    }
}

InboundMultipartPayload *NetTarget::AddToMultipartPayload(Datagram *dgram)
{
    for (InboundMultipartPayload *payload : multipart_buffer)
    {
        if (payload->id == dgram->id)
        {
            payload->Add(dgram);
            return payload;
        }
    }
    InboundMultipartPayload *new_payload = multipart_buffer.Add();
    new_payload->Start(dgram);
    new_payload->Add(dgram);
    return new_payload;
}

void NetTarget::RemoveFromMultipartBuffer(int payload_id)
{
    for (InboundMultipartPayload *payload : multipart_buffer)
    {
        if (payload->id == payload_id)
        {
            multipart_buffer.Delete(multipart_buffer.IndexOf(payload));
            break;
        }
    }
}
