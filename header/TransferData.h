#ifndef _H_TransferData_
#define _H_TransferData_

#include "ControlConnection.h"
#include "ConvertFile.h"
#include <math.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define BLOCK_FILE_NAME_LENGTH 128+1

#define EXIST_BLOCK 1
#define NOT_EXIST_BLOCK 2

typedef struct REQUEST_BLOCK_COMMAND{
	char symbol[1];
	char blockFileName[BLOCK_FILE_NAME_LENGTH];
	int fileNum;
}RequestBlockCommand;



typedef struct BLOCKPACKAGE{
	char symbol[1];
	int fileType; // block or not exist
	char blockFileName[ BLOCK_FILE_NAME_LENGTH ];
	int fileNum;
	void *blockBody;
}BlockPackage;



unsigned short BindUDPPort();

void SendBlock();
void RequestBlock( PeerInformation *PeerInf );

void SendRequestCommand( int sock, struct sockaddr_in *clntAddr ,RequestBlockCommand *command );

void TransferErrorHandling( char *errorMessage );

void ReceiveRequest();

void ReceiveBlock();

RequestBlockCommand GenerateRequestCommand(char *blockFileName, int fileNum);
RequestBlockCommand FormatRequestCommand( void *commandBuf );

BlockPackage GenerateBlockPackage( int fileType ,char *blockFileName, int fileNum, void *blockBody );



#endif // _H_TransferData_
