#include <game_project_1/net/net_types.h>
#include <game_project_1/net/os_net.hpp>


Connection::Connection(){
    hostname=null;
    port=0;
    address={0};
    socket_handle=SOCKET_ERROR;
    buffer_link=null;
    state=NOT_CONNECTED;
    message=null;
}

Connection::~Connection(){
    if(IsConnected()){Disconnect();}
    Clear();
}

void Connection::FromHostPort(wchar* host, unsigned short connect_port){
    hostname = host;
    port = htons(connect_port);
    int dns_response = OSNetwork::DNS_Lookup(hostname,port,address);
    if(dns_response != 0){
		logger::warn("Failed to resolve host %s (port %d): getaddrinfo failed with error %d\n",host,port,dns_response);
        SetState(CANT_RESOLVE,wstr::allocf(L"getaddrinfo failed with error %d\n",dns_response));
    } 
}

void Connection::FromIPAddress(NetAddress incoming_address){
    address = incoming_address;
}

void Connection::LocalLink(SynchronousBuffer* dest){
    address.SetLocalhost(true);
    address.SetValid(true);
    buffer_link=dest;
    if(buffer_link != nullptr){SetState(CONNECTED,nullptr);}
    else{SetState(DISCONNECTED,wstr::new_copy(L"Could not establish local connection- no buffer supplied"));}
}

bool Connection::Connect(Datagram* data){
    if(address.IsLocalhost()){
        buffer_link->Write((byte*)data);
        return true;
    }
    if(state != 0){return false;}
    SetState(CONNECTING,null);
    OSNetwork::Connect(data,this);
    return (state > 0);
}

void Connection::Disconnect(){
    if(address.IsLocalhost()){
        buffer_link=null;
        SetState(DISCONNECTED,null);
    }
    else{
        OSNetwork::Disconnect(this);
        SetState(DISCONNECTED,null);
    }
}

void Connection::SetState(int new_state,wchar* msg){
    state = new_state;
    if(message != nullptr){free(message);}
    message=msg;
}

bool Connection::Write(Datagram* data){
    if(state <= 0)return false;
    if(address.IsLocalhost()){
        buffer_link->Write((byte*)data);
    }
    else{
        OSNetwork::Send(data,this);
    }
    return true;
}

bool Connection::Read(Datagram* data){
    if(state <= 0)return false;
    if(address.IsLocalhost()){return false;/*local is push-push, nothing to do.*/}
    else{
        bool recv = OSNetwork::Recv(data,this);
        if(recv && state == CONNECTING){SetState(CONNECTED,null);}
        return recv;
    }
}

bool Connection::IsConnected(){
    return state > 0;
}
void Connection::Clear(){
    DEALLOCATE(hostname)
    port=0;
    address={0};
    socket_handle=SOCKET_ERROR;
    buffer_link=null;
    state=NOT_CONNECTED;
    DEALLOCATE(message)
}