#ifndef SOCKET_H_
#define SOCKET_H_
#endif /* SOCKET_H_ */

 #ifndef _POSIX_C_SOURCE
  #define _POSIX_C_SOURCE 200112L
  #endif

#include <stddef.h>
#include <string.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/socket.h>

#ifndef ERROR
#define ERROR -1
#endif

typedef struct {
	int socket;
} socket_t;

int socket_create(socket_t *self);
int socket_destroy(socket_t *self);
int socket_bind_and_listen(socket_t *self, const char *port);
int socket_connect(socket_t *self, const char* hostname, const char* port);
int socket_accept(socket_t *self, socket_t* accepted_socket);
int socket_connect(socket_t *self, const char* host_name, const char* port);
int socket_send(socket_t *self, const char* buffer, size_t length);
int socket_receive(socket_t *self, char* buffer, size_t length);
int socket_receiveTemp(socket_t *self, char* buffer, size_t size, bool *isNewMinute);
void socket_shutdown(socket_t *self);
