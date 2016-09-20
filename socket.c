/*
 * socket.c
 *
 *  Created on: 05/09/2016
 *      Author: joha
 */
#include <string.h>
#include <stddef.h>
#include "socket.h"

#define MAX_SMALL_BUF_LEN 2


/**
int getaddrinfo(const char *node, const char *service,
			   const struct addrinfo *hints,
			   struct addrinfo **res);

void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);
 * **/

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
	  printf("Error: %s\n", strerror(errno));
	  return 1;
	}

	s = bind(skt, ptr->ai_addr, ptr->ai_addrlen);
	if (s == -1) {
	  printf("Error: %s\n", strerror(errno));
	  close(skt);
	  return 1;
	}

	s = listen(skt, 20);
	if (s == -1) {
	  printf("Error: %s\n", strerror(errno));
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
	  printf("Error in getaddrinfo: %s\n", gai_strerror(status));
	  return ERROR;
	}

	addr = &ptr[0];
	skt = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if (skt == ERROR) {
	  printf("Error: %s\n", strerror(errno));
	  return ERROR;
	}

	self->socket = skt;

	status = connect(skt, addr->ai_addr, addr->ai_addrlen);
	if (status == ERROR) {
		printf("Error: %s\n", strerror(errno));
		close(self->socket);
		return ERROR;
	 }

	self->socket = skt;
	//printf("CONECTADO EN SOCKET: %d\n", self->socket);

	return status;
}

//Servidor
int socket_accept(socket_t *self, socket_t* accepted_socket){
	int status=0;
	status = accept(self->socket, NULL, NULL);   // aceptamos un cliente
	if (status == ERROR) {
		printf("Error: %s\n", strerror(errno));
	}

	accepted_socket->socket = status;
	return status;
}

//Cliente servidor
int socket_send(socket_t *self, const char* buffer, size_t length){
	int sent = 0;
	int status = 0;
	bool is_the_socket_valid = true;

	while (sent < length && is_the_socket_valid) {
		status = send(self->socket, &buffer[sent], length-sent, MSG_NOSIGNAL);

		if (status == 0) {
			is_the_socket_valid = false;
		}
		else if (status < 0) {
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
int socket_receive(socket_t *self, char* buffer, size_t length){
	int received = 0;
	int s = 0;
	bool is_the_socket_valid = true;

	while (received < length && is_the_socket_valid) {
	  s = recv(self->socket, &buffer[received], length-received, MSG_NOSIGNAL);
	  if (s == 0) { // nos cerraron el socket :(
		 is_the_socket_valid = false;
		 //printf("cerraron socket: %s\n", gai_strerror(s));
	  }
	  else if (s < 0) { // hubo un error >(
		 //printf("socket error: %s\n", gai_strerror(s));
		 is_the_socket_valid = false;
	  }
	  else {
		 received += s;
	  }
	}

	if (is_the_socket_valid) {
		//printf("Error in recv: %s\n", gai_strerror(s));
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
