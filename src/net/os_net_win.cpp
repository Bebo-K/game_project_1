#ifdef _WIN32

#ifndef WIN_NET_H
#define WIN_NET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib") 

#include "os_net.h"
#include "network.h"
#include "../log.h"


bool winsock_initialized=false;

bool OSNetwork::init(){
	if(winsock_initialized)return true;
	WSADATA wsaData;//has specifics like max datagram size and max connections, but otherwise unneeded.
	if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
			logger::exception("Failed to initialize networking. WSAStartup error code: %d",WSAGetLastError());
			return false;
		}	
	winsock_initialized=true;
	return true;
}

Socket OSNetwork::bind_to_port(short port){
	SOCKET socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_handle == INVALID_SOCKET){
		logger::exception("Failed to create a local socket. WinSock2 socket() error code: %d",WSAGetLastError());
		return INVALID_SOCKET;
	}
	sockaddr_in bind_address;
	bind_address.sin_family = AF_INET;
	bind_address.sin_port = ntohs(port);
	bind_address.sin_addr.s_addr = htonl(INADDR_ANY);
	int bind_result = bind(socket_handle, (SOCKADDR *) &bind_address, sizeof (bind_address));
	if(bind_result != 0){
		logger::exception("Failed to bind to port. WinSock2 bind() error result: %d",bind_result);
		return INVALID_SOCKET;
	}
	return socket_handle;
}

Socket OSNetwork::connect(Packet* connect_packet,wchar_t* hostname,short port){
	addrinfoW	dns_hints;
	addrinfoW*	target_address;

	memset(&dns_hints, 0, sizeof(dns_hints));
    dns_hints.ai_socktype = SOCK_DGRAM;
    dns_hints.ai_protocol = IPPROTO_UDP;
	dns_hints.ai_family = AF_INET;

	int dns_response = GetAddrInfoW(hostname,wstr::write_integer_string(port),&dns_hints,&target_address);
    if ( dns_response != 0 ) {
		logger::warn("Failed to resolve host %s (port %d): getaddrinfo failed with error %d\n",hostname,port,dns_response);
		FreeAddrInfoW(target_address);
        return false;
    }


	Socket connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (connect_socket == INVALID_SOCKET) {
		logger::exception("Failed to create a local socket on connect. WinSock2 socket() error code: %d",WSAGetLastError());
    	return INVALID_SOCKET;
	}

	//Sendto will internally mark port as "bound", and can be listen from afterwards
	int send_result = sendto(connect_socket,(const char*)connect_packet,connect_packet->length,0,target_address->ai_addr,target_address->ai_addrlen); 
	int send_error=0;
	if(send_result == SOCKET_ERROR){
		send_error = WSAGetLastError();
		/*
		while(target_address->ai_next != nullptr && send_result == SOCKET_ERROR){
			target_address = target_address->ai_next;
			send_result = sendto(connect_socket,(const char*)connect_packet,connect_packet->length,0,target_address->ai_addr,target_address->ai_addrlen); 
			if(send_result != SOCKET_ERROR){
				return connect_socket
			}
		}
		*/
		return SOCKET_ERROR;
	}
	FreeAddrInfoW(target_address);

	/*We might not need client port info but if we do for some reason:
		_CSADDR_INFO local_addr_info;
		getsockopt(connect_socket,SOL_SOCKET,SO_BSP_STATE ,(char*)&local_addr_info,(int)sizeof(local_addr_info));
	*/

	return connect_socket;
}

bool OSNetwork::disconnect(Socket){

}

bool OSNetwork::send(Packet* packet, Socket dest){


}
bool OSNetwork::recv(Socket* bound_socket){

}
    
void OSNetwork::destroy(){
	winsock_initialized=false;
	WSACleanup();
}





#endif 
#endif