#include <string.h>
#include <stddef.h>
#include "socket.h"

int socket_create(socket_t *self){
	self->socket = 1;
	return 0;
}

int socket_destroy(socket_t *self){
	return 0;
}

int socket_bind_and_listen(socket_t *self, const char *port){
	int s = 0;
	struct addrinfo hints;
	struct addrinfo *ptr;
	int skt;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

	s = getaddrinfo(NULL, port, &hints, &ptr);

	if (s != 0) {
	  return ERROR;
	}

	skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	self->socket = skt;

	if (skt == -1) {

	  return 1;
	}

	s = bind(skt, ptr->ai_addr, ptr->ai_addrlen);
	if (s == -1) {
	  close(skt);
	  return 1;
	}

	s = listen(skt, 20);
	if (s == -1) {
	  close(skt);
	  return 1;
	}

	return skt;

}


//Cliente
int socket_connect(socket_t *self, const char* hostname, const char* port){
	int status = 0, skt = 0;
	struct addrinfo *ptr;
	struct addrinfo *addr;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

	status = getaddrinfo(hostname, port, &hints, &ptr);

	if (status != 0) {
	  return ERROR;
	}

	addr = &ptr[0];
	skt = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if (skt == ERROR) {
	  return ERROR;
	}

	self->socket = skt;

	status = connect(skt, addr->ai_addr, addr->ai_addrlen);
	if (status == ERROR) {
		close(self->socket);
		return ERROR;
	 }

	self->socket = skt;

	return status;
}

//Servidor
int socket_accept(socket_t *self, socket_t* accepted_socket){
	int status=0;
	status = accept(self->socket, NULL, NULL);   // aceptamos un cliente
	if (status == ERROR) {
		return ERROR;
	}

	accepted_socket->socket = status;
	return status;
}

//Cliente servidor
int socket_send(socket_t *self, const char* buffer, size_t size){
	int sent = 0;
	int status = 0;
	bool is_the_socket_valid = true;

	while (sent < size && is_the_socket_valid) {
		status = send(self->socket, &buffer[sent], size-sent-1, MSG_NOSIGNAL);

		if (status <= 0) {
			is_the_socket_valid = false;
		}
		else {
			sent += status;
		}
	}

	if (is_the_socket_valid) {
		return sent;
	}
	else {
		return -1;
	}

}

//Cliente servidor
int socket_receive(socket_t *self, char* buffer, size_t size){
	int received = 0;
	int bytes = 0;
	bool is_the_socket_valid = true;

	memset(buffer ,0 , size);
	while (size> received && is_the_socket_valid) {
		bytes = recv(self->socket, &buffer[received], size-received, MSG_NOSIGNAL);
		if (received && buffer[received]== " "){
			break;

		}
		if ( bytes <= 0) {
			is_the_socket_valid = false;
		}
		else {
			received += bytes;
		}
	}

	if (is_the_socket_valid) {
		return received;
	}
	else {
	  return -1;
	}
}

int socket_receiveTemp(socket_t *self, char* buffer, size_t size){
	int received = 0;
	int bytes = 0;
	bool is_the_socket_valid = true;

	memset(buffer ,0 , size);
	while (size> received && is_the_socket_valid) {
		bytes = recv(self->socket, &buffer[received],1, MSG_NOSIGNAL);
		if (received && (buffer[received]== ' ' || buffer[received]== '\n' )){
			break;

		}
		if ( bytes <= 0) {
			is_the_socket_valid = false;
		}
		else {
			received += bytes;
		}
	}

	if (is_the_socket_valid) {
		return received;
	}
	else {
	  return -1;
	}
}




//Cliente servidor
void socket_shutdown(socket_t *self){
	shutdown(self->socket, SHUT_RDWR);
	close(self->socket);
}
