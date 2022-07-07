#ifdef _WIN32

#ifndef WIN_NET_H
#define WIN_NET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
//#pragma comment(lib, "Ws2_32.lib") 

#include <game_project_1/net/os_net.hpp>
#include <game_project_1/net/network.hpp>
#include <game_project_1/log.hpp>
#include <game_project_1/config.hpp>

using namespace OSNetwork;

void set_state_from_error(Connection* conn);


bool Init(){
	WSADATA wsaData;//has specifics like max datagram size and max connections, but otherwise unneeded.
	if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
			logger::exception("Failed to initialize networking. WSAStartup error code: %d",WSAGetLastError());
        	WSACleanup();
			return false;
		}	
	return true;
}

void Destroy(){
	WSACleanup();
}

int DNS_lookup(wchar* hostname, unsigned short port,NetAddress& addr){
	addrinfoW*	target_address;
	addrinfoW	dns_hints = {0};
		dns_hints.ai_socktype = SOCK_DGRAM;
		dns_hints.ai_protocol = IPPROTO_UDP;
		dns_hints.ai_family = AF_INET6;
	wchar* port_string =  wstr::write_integer_string(port);
	int dns_response = GetAddrInfoW(hostname,port_string,&dns_hints,&target_address);
	free(port_string);
    if ( dns_response != 0 ) {
		FreeAddrInfoW(target_address);
        return dns_response;
    }

	if(target_address->ai_family == AF_INET6){
		sockaddr_in6* ipv6 = (sockaddr_in6*)target_address->ai_addr;
		addr.SetIpv6(true);
		addr.SetValid(true);
		
		memcpy(addr.ipv6,&ipv6->sin6_addr,16);
		addr.ipv6_flowinfo = ipv6->sin6_flowinfo;
		addr.port = ipv6->sin6_port;
		addr.ipv6_scopeid = ipv6->sin6_scope_id;
		addr.ipv6_scope = ipv6->sin6_scope_struct.Value;
	}
	else if(target_address->ai_family == AF_INET){
		sockaddr_in* ip = (sockaddr_in*)target_address->ai_addr;
		addr.SetIpv6(false);
		addr.SetValid(true);
		
		memcpy(addr.ipv4,&ip->sin_addr,4);
		addr.port=ip->sin_port;
	}

	//TODO: handle target_address->ai_next + multiple attempts

	FreeAddrInfoW(target_address);
	return 0;
}

void connect(Datagram* data,Connection* conn){
	Socket s = socket(conn->address.IsIpv6()? AF_INET6 : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) {
		conn->SetState(Connection::NETWORK_UNAVAILABLE,
			wstr::allocf(L"Failed to create a local socket on connect. WinSock2 socket() error code: %d",WSAGetLastError()) );		
		return;
	}
	conn->socket_handle = s;
	int send_result=0;
	if(conn->address.IsIpv6()){
		sockaddr_in6 addr = {0};
		memcpy(&addr.sin6_addr,conn->address.ipv6,16);
		addr.sin6_family = AF_INET6;
		addr.sin6_flowinfo= conn->address.ipv6_flowinfo;
		addr.sin6_port = conn->address.port;
		addr.sin6_scope_id= conn->address.ipv6_scopeid;
		addr.sin6_scope_struct.Value= conn->address.ipv6_scope;

		send_result = sendto(s,(const char*)data,data->length,0,(sockaddr*)&addr,sizeof(sockaddr_in6)); 
	}
	else{
		sockaddr_in addr = {0};
		memcpy(&addr.sin_addr,conn->address.ipv4,4);
		addr.sin_family = AF_INET;
		addr.sin_port = conn->address.port;

		send_result = sendto(s,(const char*)data,data->length,0,(sockaddr*)&addr,sizeof(sockaddr_in)); 
	}

	//Sendto will internally mark port as "bound", and can be listened from afterwards
	if(send_result == SOCKET_ERROR){
		set_state_from_error(conn);
		conn->socket_handle = INVALID_SOCKET;
	}
	else if(send_result != data->Size()){
		conn->SetState(Connection::UNKNOWN_ERROR,wstr::new_copy(L"Connect packet was not completely sent"));
		conn->socket_handle = INVALID_SOCKET;
	}
}

void disconnect(Connection* conn){
	closesocket(conn->socket_handle);
	conn->socket_handle = INVALID_SOCKET;
}

bool send(Datagram* dgram, Connection* conn){
	int result = send(conn->socket_handle,(char*)dgram, dgram->length, 0 );
    if (result == SOCKET_ERROR) {
		set_state_from_error(conn);
        return false;
    }
	return true;
}

bool recv(Datagram* dgram, Connection* conn){
	unsigned long available;
	if(ioctlsocket(conn->socket_handle,FIONREAD,&available) != 0){
		conn->SetState(Connection::UNKNOWN_ERROR,wstr::allocf(L"Failed to get listener socket status, error code %x",WSAGetLastError()));
        return false;
	}
	if(available > 0){return false;}

	int result = recv(conn->socket_handle,(char*)dgram, dgram->length, 0 );
    if (result == SOCKET_ERROR) {
		set_state_from_error(conn);
        return false;
    }
	return true;
}

Socket bind_to_port(unsigned short port){
	Socket result = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (result == INVALID_SOCKET){
		logger::exception("Failed to create a local socket. WinSock2 socket() error code: %d",WSAGetLastError());
		return INVALID_SOCKET;
	}
	sockaddr_in6 bind_address;
	bind_address.sin6_family = AF_INET6;
	bind_address.sin6_port = ntohs(port);
	bind_address.sin6_addr = in6addr_any;
	int bind_result = bind(result, (SOCKADDR *) &bind_address, sizeof (bind_address));
	if(bind_result != 0){
		logger::exception("Failed to bind to port. WinSock2 bind() error result: %d",bind_result);
		return INVALID_SOCKET;
	}
	return result;
}

void unbind(Socket* listen_socket){
	closesocket(*listen_socket);
	*listen_socket = INVALID_SOCKET;
}

int listen(Datagram* dgram, Socket socket,NetAddress* source_addr){
	unsigned long available;
	if(ioctlsocket(socket,FIONREAD,&available) != 0){
		logger::exception("Failed to get listener socket status, error code %d\n",WSAGetLastError());
        return -1;
	}
	if(available <= 0){return 0;}

	sockaddr_storage remote_addr;
	int remote_addr_size = sizeof(sockaddr_storage);

	int result = recvfrom(socket,(char*)dgram, sizeof(Datagram), 0 ,(sockaddr*)&remote_addr,&remote_addr_size);
    if (result == SOCKET_ERROR) {
		logger::exception("Failed to listen on server socket, error code %d",WSAGetLastError());
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

void set_state_from_error(Connection* conn){
	int error_code = WSAGetLastError();
	switch(error_code){
		case 0: break; 
		case WSAENETDOWN:{
			conn->SetState(Connection::NETWORK_UNAVAILABLE,wstr::allocf(L"The network is unavailable"));
			break;}
		case WSAECONNRESET:{
			conn->SetState(Connection::ABORTED,wstr::allocf(L"Connection reset"));
			break;}
		case WSAETIMEDOUT:{
			conn->SetState(Connection::TIMED_OUT,wstr::allocf(L"Connection timed out"));
			break;}
		case WSAEADDRNOTAVAIL:{
			conn->SetState(Connection::INVALID,
				(conn->hostname != nullptr)? wstr::allocf(L"The remote host %S is not a valid address",conn->hostname)
											: wstr::allocf(L"The remote address is invalid"));
			break;}
		case WSAEHOSTUNREACH:{
			conn->SetState(Connection::INVALID,
				(conn->hostname != nullptr)? wstr::allocf(L"Remote host unreachable %S",conn->hostname)
											: wstr::allocf(L"Remote host is unreachable"));
			break;}
		case WSAENETUNREACH:{
			conn->SetState(Connection::NETWORK_UNAVAILABLE,wstr::allocf(L"Network is unreachable from this host"));
			break;}
		case WSAEDESTADDRREQ:{
			conn->SetState(Connection::INVALID,wstr::allocf(L"No destination address"));
			break;}
		default: {
			conn->SetState(Connection::UNKNOWN_ERROR,wstr::allocf(L"An unknown error has occured (%d)",error_code));
			break;
		}
	}

	if(config::debug_net && error_code != 0){
		char* char_msg = cstr::from_wstr(conn->message);
		if(conn->hostname != nullptr){
			logger::debug("Error for connection to address %s",conn->hostname);	
		}
		logger::debug(char_msg);
		free(char_msg);
	}
}




#endif 
#endif