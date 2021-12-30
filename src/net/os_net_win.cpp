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
void set_socket_state_from_error(Socket dest);

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
	if(send_result == SOCKET_ERROR){
		set_socket_state_from_error(SOCKET_ERROR);
		FreeAddrInfoW(target_address);
		return SOCKET_ERROR;
	}
	else if(send_result != connect_packet->length){
		set_state_for_socket(connect_socket,OSNetwork::UNKNOWN_ERROR,L"join request packet was not completely sent");
	}
	FreeAddrInfoW(target_address);


	return connect_socket;
}

bool OSNetwork::disconnect(Socket dest){
	closesocket(dest);
}

bool OSNetwork::send_packet(Packet* packet, Socket dest){
	int result = send( dest,(char*)packet, packet->length, 0 );
    if (result == SOCKET_ERROR) {
		set_socket_state_from_error(dest);
		int send_failure = WSAGetLastError();
		switch(send_failure){//TODO: handling timeout/connection lost disconnects
			case WSAENOTCONN:		break;
			default:break;
		}
        wprintf(L"send failed with error: %d\n", send_failure);
        return false;
    }
	return true;
}

bool OSNetwork::recv_packet(Socket* bound_socket){


}
    
void OSNetwork::destroy(){
	winsock_initialized=false;
	WSACleanup();
}

void OSNetwork::set_state_for_socket(Socket target_socket,SocketState state_code, wchar* msg){
	//TODO
}

int OSNetwork::get_state_for_socket(Socket target_socket,wchar* message){

}

void set_socket_state_from_error(Socket dest,wchar* address_str){
	int error_code = WSAGetLastError();
	OSNetwork::SocketState state = OSNetwork::CONNECTED;
	wchar* msg = nullptr;
	
	switch(error_code){
		case 0: break; 
		case WSAENETDOWN:{
			state = OSNetwork::NETWORK_UNAVAILABLE;
			msg = wstr::allocf(L"The network is unavailable");
			break;}
		case WSAECONNRESET:{
			state = OSNetwork::ABORTED;
			msg = wstr::allocf(L"Connection reset");
			break;}
		case WSAETIMEDOUT:{
			state = OSNetwork::TIMED_OUT;
			msg = wstr::allocf(L"Connection timed out");
			break;}
		case WSAEADDRNOTAVAIL:{
			state = OSNetwork::INVALID;
			msg = (address_str != nullptr)? wstr::allocf(L"The remote host %s is not a valid address",address_str)
			: wstr::allocf(L"The remote address is invalid");
			break;}
		case WSAEHOSTUNREACH:{
			state = OSNetwork::INVALID;
			msg = (address_str != nullptr)? wstr::allocf(L"Remote host unreachable %s",address_str)
			: wstr::allocf(L"Remote host is unreachable");
			break;}
		case WSAENETUNREACH:{
			state = OSNetwork::NETWORK_UNAVAILABLE;
			msg = wstr::allocf(L"Network is unreachable from this host");
			break;}
		case WSAEDESTADDRREQ:{
			state = OSNetwork::INVALID;
			msg = wstr::allocf(L"No destination address");
			break;}
		default: {
			state = OSNetwork::UNKNOWN_ERROR;
			msg = wstr::allocf(L"An unknown error has occured (%d)",error_code);
		}
	}
	if(state != OSNetwork::CONNECTED){
		OSNetwork::set_state_for_socket(dest,state,msg);
	}
}




#endif 
#endif