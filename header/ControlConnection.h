#ifndef _H_ControlConnection_
#define _H_ControlConnection_

#include "HashCipher.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define MAXPENDING 5

#define SOCKET_RECEIVEQ_SIZE 1
#define SOCKET_SENDQ_SIZE 2
#define	RSA_PUBLIC_KEY 3
#define AES_KEY 4
#define UDP_PORT 5
#define REQUEST_BLOCK 6

#define COMMAND_SYMBOL $
#define COMMAND_LENGTH sizeof(int)
#define FILE_SIZE_LENGTH sizeof(size_t)

#define TIMEOUT_SECS 2 // (s)


typedef struct PEERINFORMATION{

	int TCPPeerSock; // control connection
	int UDPPeerSock; // transfer data
	char *PeerIP;
	unsigned short PeerUDPPort;
	int PeerSendQSize;
	int PeerRecvQSize;
	EVP_PKEY *PeerRSAPublicKey;

}PeerInformation;


typedef struct CONTROLCOMMAND{
	char symbol[1];
	int command;
	size_t fileSize;
	void *file;
}ControlCommand;


int ClientConnection(char *servIP, unsigned short servPort);
int ServerConnection(unsigned short servPort);

void SetSIGIO(struct sigaction *handler);
void SetNonBlocking(int sock);

void SendControlMessage(int socket);

void ConnectionErrorHandling(char *errorMessage);

void SIGIOHandler(int signalType);

void SendCommand(int sock ,int command, const size_t fileSize, void *file);

void SendControlMessage();

void GetSocketQSize(int sock, int *SendQSize, int *RecvQSize);

void SetSocketQSize(int sock, int SendQSize, int RecvQSize);

void ReceiveCommand(int sock);

void HandleCommand(const int controlMessage, const size_t fileSize, void *file);

void ReceiveCommandTimeOut();


#endif // _H_ControlConnection_