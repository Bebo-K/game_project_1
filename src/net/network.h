#ifndef NETWORK_H
#define NETWORK_H

#include "packet.h"
#include "os_net.h"
#include "../threads.h"
#include "../struct/arrays.h"

//Some notes
//The largest safe UDP packet size over the internet is 508 bytes
//When fragmented, UDP packets can combine into a max 67KB payload


class NetTarget{
    private:
    char* hostname;
    unsigned short port;

    public:
    Socket os_socket;
    SynchronousBuffer read_buffer;
    SynchronousBuffer write_buffer;
    DynamicArray packet_assembly_buffer;//for inbound reliable packets

    bool connected;
    int latency;

    void SetAddress(char* hostname);
    bool ResolveAddress();
    NetTarget();
    ~NetTarget();
};


class Network{
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

};



















#endif