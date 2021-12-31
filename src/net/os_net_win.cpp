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
#include "../config.h"

void set_state_from_error(NetTarget* target);
void handle_listen_error(ServerNetwork* server_net);

bool OSNetwork::Init(){
	WSADATA wsaData;//has specifics like max datagram size and max connections, but otherwise unneeded.
	if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
			logger::exception("Failed to initialize networking. WSAStartup error code: %d",WSAGetLastError());
        	WSACleanup();
			return false;
		}	
	return true;
}

void OSNetwork::Destroy(){
	WSACleanup();
}

ip_address OSNetwork::DNS_lookup(wchar* hostname, unsigned short port){
	ip_address ret = {0};
	addrinfoW*	target_address;

	addrinfoW	dns_hints = {0};
    dns_hints.ai_socktype = SOCK_DGRAM;
    dns_hints.ai_protocol = IPPROTO_UDP;
	dns_hints.ai_family = AF_INET6;
	int dns_response = GetAddrInfoW(hostname,wstr::write_integer_string(port),&dns_hints,&target_address);
    if ( dns_response != 0 ) {
		logger::warn("Failed to resolve host %s (port %d): getaddrinfo failed with error %d\n",hostname,port,dns_response);
		FreeAddrInfoW(target_address);
        return ret;
    }

	if(target_address->ai_family == AF_INET6){
		sockaddr_in6* ipv6 = (sockaddr_in6*)target_address->ai_addr;
		ret.set_ipv6(true);
		ret.set_valid(true);
		
		memcpy(ret.ipv6,&ipv6->sin6_addr,16);
		ret.ipv6_flowinfo = ipv6->sin6_flowinfo;
		ret.port = ipv6->sin6_port;
		ret.ipv6_scopeid = ipv6->sin6_scope_id;
		ret.ipv6_scope = ipv6->sin6_scope_struct.Value;
	}
	else if(target_address->ai_family == AF_INET){
		sockaddr_in* ip = (sockaddr_in*)target_address->ai_addr;
		ret.set_ipv6(false);
		ret.set_valid(true);
		
		memcpy(ret.ipv4,&ip->sin_addr,4);
		ret.port=ip->sin_port;
	}

	//TODO: handle target_address->ai_next + multiple attempts

	FreeAddrInfoW(target_address);
}

bool OSNetwork::connect(Packet* connect_packet,NetTarget* target){
	int send_result;

	if(target->address.is_localhost()){
		target->state_id=NetTarget::CONNECTED_LOCAL;
		return true;
	}
	
	target->socket = socket(target->address.is_ipv6()? AF_INET6 : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (target->socket == INVALID_SOCKET) {
		logger::exception("Failed to create a local socket on connect. WinSock2 socket() error code: %d",WSAGetLastError());
		return false;
	}

	if(target->address.is_ipv6()){
		sockaddr_in6 addr = {0};
		memcpy(&addr.sin6_addr,target->address.ipv6,16);
		addr.sin6_family = AF_INET6;
		addr.sin6_flowinfo= target->address.ipv6_flowinfo;
		addr.sin6_port = target->address.port;
		addr.sin6_scope_id= target->address.ipv6_scopeid;
		addr.sin6_scope_struct.Value= target->address.ipv6_scope;

		send_result = sendto(target->socket,(const char*)connect_packet,connect_packet->length,0,(sockaddr*)&addr,sizeof(sockaddr_in6)); 
	}
	else{
		sockaddr_in addr = {0};
		memcpy(&addr.sin_addr,target->address.ipv4,4);
		addr.sin_family = AF_INET;
		addr.sin_port = target->address.port;

		send_result = sendto(target->socket,(const char*)connect_packet,connect_packet->length,0,(sockaddr*)&addr,sizeof(sockaddr_in)); 
	}

	//Sendto will internally mark port as "bound", and can be listened from afterwards
	if(send_result == SOCKET_ERROR){
		set_state_from_error(target);
		target->socket = INVALID_SOCKET;
		return false;
	}
	else if(send_result != connect_packet->length){
		target->state_id = NetTarget::UNKNOWN_ERROR;
		target->SetStateMsg(wstr::allocf(L"Connect packet was not completely sent"));
		target->socket = INVALID_SOCKET;
		return false;
	}
	
	return true;
}

void OSNetwork::disconnect(NetTarget* target){
	closesocket(target->socket);
	target->socket = INVALID_SOCKET;
}

bool OSNetwork::send_packet(Packet* packet,NetTarget* target){
	int result = send(target->socket,(char*)packet, packet->length, 0 );
    if (result == SOCKET_ERROR) {
		set_state_from_error(target);
        return false;
    }
	return true;
}

bool OSNetwork::recv_packet(Packet* packet,NetTarget* target){
	unsigned long available;
	int sockstat =ioctlsocket(target->socket,FIONREAD,&available);
	if(available > 0){return false;}

	int result = recv(target->socket,(char*)packet, packet->length, 0 );
    if (result == SOCKET_ERROR) {
		set_state_from_error(target);
        return false;
    }
	return true;
}



Socket OSNetwork::bind_to_port(short port){
	Socket result = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (result == INVALID_SOCKET){
		logger::exception(wstr::allocf("Failed to create a local socket. WinSock2 socket() error code: %d",WSAGetLastError()));
		return INVALID_SOCKET;
	}
	sockaddr_in6 bind_address;
	bind_address.sin6_family = AF_INET6;
	bind_address.sin6_port = ntohs(port);
	bind_address.sin6_addr = in6addr_any;
	int bind_result = bind(result, (SOCKADDR *) &bind_address, sizeof (bind_address));
	if(bind_result != 0){
		logger::exception(wstr::allocf("Failed to bind to port. WinSock2 bind() error result: %d",bind_result));
		return INVALID_SOCKET;
	}
	return true;
}

void OSNetwork::unbind(Socket* listen_socket){
	closesocket(*listen_socket);
	*listen_socket = INVALID_SOCKET;
}

int OSNetwork::listen(Packet* packet, Socket socket,ip_address* source_addr){
	unsigned long available;
	int sockstat =ioctlsocket(server->socket,FIONREAD,&available);
	if(available > 0){return 0;}

	sockaddr_storage remote_addr;
	int remote_addr_size = sizeof(sockaddr_storage);

	int result = recvfrom(server->socket,(char*)packet, sizeof(Packet), 0 ,(sockaddr*)&remote_addr,&remote_addr_size);
    if (result == SOCKET_ERROR) {
		logger::exception(wstr::allocf("Failed to listen on server socket %x",WSAGetLastError()));
        return -1;
    }
	
	if(source_addr == nullptr){return result;}

	if(remote_addr.ss_family == AF_INET){
		sockaddr_in* remote_ipv4 = (sockaddr_in*)&remote_addr;
		source_addr->Clear();
		source_addr->SetValid(true);
		source_addr->SetIpv6(false);
		memcpy(source_addr->ipv4,&remote_ipv4->sin_addr,4);
		source_addr->port = remote_ipv4->sin_port;
	}
	else if(remote_addr.ss_family == AF_INET6){
		sockaddr_in6* remote_ipv6 = (sockaddr_in6*)&remote_addr;
		source_addr->SetValid(true);
		source_addr->SetIpv6(true);
		memcpy(source_addr->ipv6,&remote_ipv6->sin6_addr,16);
		source_addr->ipv6_flowinfo = remote_ipv6->sin6_flowinfo;
		source_addr->port = remote_ipv6->sin6_port;
		source_addr->ipv6_scopeid = remote_ipv6->sin6_scope_id;
		source_addr->ipv6_scope = remote_ipv6->sin6_scope_struct.Value;
	}
	return result;
}


void set_state_from_error(NetTarget* target){
	int error_code = WSAGetLastError();

	switch(error_code){
		case 0: break; 
		case WSAENETDOWN:{
			target->state_id = NetTarget::NETWORK_UNAVAILABLE;
			target->SetStateMsg(wstr::allocf(L"The network is unavailable"));
			break;}
		case WSAECONNRESET:{
			target->state_id = NetTarget::ABORTED;
			target->SetStateMsg(wstr::allocf(L"Connection reset"));
			break;}
		case WSAETIMEDOUT:{
			target->state_id = NetTarget::TIMED_OUT;
			target->SetStateMsg(wstr::allocf(L"Connection timed out"));
			break;}
		case WSAEADDRNOTAVAIL:{
			target->state_id = NetTarget::INVALID;
			target->SetStateMsg(
				(target->hostname != nullptr)? wstr::allocf(L"The remote host %s is not a valid address",target->hostname)
					: wstr::allocf(L"The remote address is invalid"));
			break;}
		case WSAEHOSTUNREACH:{
			target->state_id = NetTarget::INVALID;
			target->SetStateMsg(
				(target->hostname != nullptr)? wstr::allocf(L"Remote host unreachable %s",target->hostname)
					: wstr::allocf(L"Remote host is unreachable"));
			break;}
		case WSAENETUNREACH:{
			target->state_id = NetTarget::NETWORK_UNAVAILABLE;
			target->SetStateMsg(wstr::allocf(L"Network is unreachable from this host"));
			break;}
		case WSAEDESTADDRREQ:{
			target->state_id = NetTarget::INVALID;
			target->SetStateMsg(wstr::allocf(L"No destination address"));
			break;}
		default: {
			target->state_id = NetTarget::UNKNOWN_ERROR;
			target->SetStateMsg(wstr::allocf(L"An unknown error has occured (%d)",error_code));
			break;
		}
	}

	if(config::debug_net && error_code != 0){
		char* char_msg = cstr::from_wstr(target->state_msg);
		if(target->hostname != nullptr){
		logger::debug("Error for connection to address %s",target->hostname);	
		}
		logger::debug(char_msg);
		free(char_msg);
	}
}




#endif 
#endif