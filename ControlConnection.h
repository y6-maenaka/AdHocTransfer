#ifndef _H_ControlConnection_
#define _H_ControlConnection_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#define MAXPENDING 5

int ClientConnection(char *servIP, unsigned short servPort);
int ServerConnection(unsigned short servPort);

void SendControlMessage(int socket);

void ConnectionErrorHandling(char *errorMessage);

void SIGIOHandler(int signalType);

void SendCommand();

#endif // _H_ControlConnection_
