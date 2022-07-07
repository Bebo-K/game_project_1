#ifndef NET_TARGET_H
#define NET_TARGET_H



#include <game_project_1/net/net_types.h>

#include <game_project_1/net/payload.hpp>
#include <game_project_1/types/str.hpp>
#include <game_project_1/types/pool.hpp>
#include <game_project_1/threads.hpp>


class NetTarget{
    private:
    const static int RELIABLE_ID_HISTORY=10;
    public:
    Connection      connection;
    int             last_success;
    int             latency;
    int             sent_reliables[RELIABLE_ID_HISTORY];
    int             ackd_reliables[RELIABLE_ID_HISTORY];

    Pool<OutboundReliablePayload> reliable_buffer;
    Pool<InboundMultipartPayload> multipart_buffer;

    SynchronousBuffer inbound_buffer;//buffer of packets recieved
    SynchronousBuffer outbound_buffer;//buffer of packets to send

    void ForLocal(SynchronousBuffer* buffer_link);
    void ForAddress(wchar* hostname,unsigned short port);
    void ForAddress(wchar* host_and_port);
    void ForAddress(NetAddress address);

    void Clear();

    void Update();

    void Send(Payload payload);
    Payload Recieve();
    bool OnRecieve(Payload payload);

    bool IsConnected();

    bool SendAck(int ack_id);//sends an acknowledgement packet for the payload ID
    void SendDisconnect(wchar* reason);//sends a disconnect packet and closes the connection

    NetTarget();
    ~NetTarget();

    private:

    int NewReliableID();
    void AddToReliableBuffer(Payload payload);
    void RemoveFromReliableBuffer(int reliable_id);

    InboundMultipartPayload* AddToMultipartPayload(Datagram* dgram);//gets or creates a multipartpayload in the buffer for the packet's ID and inserts it.
    void RemoveFromMultipartBuffer(int packet_id);//for purging/expiry, data pointer is freed
};




#endif