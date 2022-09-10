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

#define SOCKETQ_SIZE 1
#define	RSA_PUBLIC_KEY 2
#define AES_KEY 3
#define UDP_PORT 4

#define COMMAND_SYMBOL $
#define COMMAND_LENGTH 4
#define FILE_SIZE_LENGTH 8

#define TIMEOUT_SECS 2 // (s)


typedef struct PEERINFORMATION{

	int TCPPeerSock;
	int UDPPeerSock;
	char PeerIP[4];
	int PeerSendQSize;
	int PeerRecvQSize;

}PeerInformation;


int ClientConnection(char *servIP, unsigned short servPort);
int ServerConnection(unsigned short servPort);

void SendControlMessage(int socket);

void ConnectionErrorHandling(char *errorMessage);

void SIGIOHandler(int signalType);

void SendCommand();

void SendControlMessage();

void GetSocketQSize(int sock, int *SendQSize, int *RecvQSize);

void ReceiveCommand(int sock);

void HandleCommand(const int controlMessage, const int fileSize,unsigned char *file);

void ReceiveCommandTimeOut();

#endif // _H_ControlConnection_
