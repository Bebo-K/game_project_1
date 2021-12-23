#ifndef NETWORK_H
#define NETWORK_H

#include "packet.h"
#include "os_net.h"
#include "../threads.h"
#include "../struct/arrays.h"

#include <iostream>

//Some notes
//The largest safe UDP packet size over the internet is 508 bytes
//When fragmented, UDP packets can combine into a max 67KB payload


class NetTarget{
    private:
    char* hostname;
    unsigned short port;

    void ResizeReliableBuffer(int new_size);
    void ResizeMultipartBuffer(int new_size);

    public:
    Socket os_socket;
    ReliablePacketEnvelope* outbound_buffer;//for outbound reliable packets
    int outbound_buffer_len;
    MultipartPayload* inbound_buffer;//for inbound multipart packets
    int inbound_buffer_len;
    SynchronousBuffer read_buffer;
    SynchronousBuffer write_buffer;

    bool connected;
    int latency;

    void SetAddress(char* hostname);
    bool ResolveAddress();
    
    void AddToReliableBuffer(Packet* packet);
    MultipartPayload* GetMultipartPayload(MultipartPacket* packet);

    NetTarget();
    ~NetTarget();
};


class Network{
    private:
    byte* payload_buffer;
    long payload_buffer_len;


    public:
    bool offline;//local communication only
    Socket listener_socket;
    bool listener_enabled;
    int listener_port;
    int target_count;
    NetTarget* targets;

    void Initialize(int max_targets);//OS initialization code, starts networking thead
    void Destroy();//OS deinitialization, disconnection and cleanup
    void StartListener(short port);//enables listener on net thread
    int AddTarget(char* address);//initializes a new target for network thead. Returns target id(index)

    void WriteToTarget(int target_id,Payload payload);
    Payload ReadFromTarget(int target_id);
    Payload OpenPayload(int type, int len, byte* source);
    void ClosePayload();
};



















#endif