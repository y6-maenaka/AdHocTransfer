#include "header/HashCipher.h"
#include "header/ControlFile.h"
#include "header/ControlConnection.h"
#include "header/ConvertFile.h"
#include "header/TransferData.h"

#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define SIGRTMIN 33

bool transferDoneFlag = false;


struct ThreadArgs{
	PeerInformation PeerInf;
	pthread_t mainThreadID;
};

void *TransferThread( struct ThreadArgs threadArgs  );

int AllowRequestBlockDoneSignal();

void ReceiveRequestBlockSequenceDoneSignal();


int main(int argc, char *argv[]){

	LoadConfig();
	PeerInformation *PeerInf = GetPeerInformation();

	//ServerConnection();
	//ReceiveBlock();
	
	unsigned short servPort = atoi( argv[1]);
	
	PeerInf->TCPPeerSock = ServerConnection( servPort );

	printf("serv port -> %d\n", PeerInf->PeerUDPPort);

	sleep(15);
	//struct ThreadArgs threadArgs;
	//threadArgs.PeerInf = *PeerInf;
	//threadArgs.mainThreadID = pthread_self();
	// スレッドを作成する前にUDPポートを確認しておく
	//pthread_t threadID;
	//pthread_create( &threadID, NULL, TransferThrea, (void *)threadArgs);
	
	//while(1){
		//pause();
		//if ( transferDoneFlag ) break;
	//}

	//BlockToFile();
	return 0;

};



void *TransferThread( struct ThreadArgs threadArgs ){
	pthread_detach( pthread_self());

	threadArgs.PeerInf.UDPPeerSock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP); // socketの作成
																																		
	// ======= ノンブロッキング設定 =======
	struct sigaction handler;
	handler.sa_handler = ReceiveBlock;
	SetNonBlocking( threadArgs.PeerInf.UDPPeerSock );
	SetSignal( &handler, SIGIO );

	RequestBlock( &threadArgs.PeerInf );

	pthread_kill( threadArgs.mainThreadID, SIGRTMIN+1);


	return (NULL);
};


int AllowRequestBlockDoneSignal(){
	struct sigaction handler;
	handler.sa_handler = ReceiveRequestBlockSequenceDoneSignal;
	SetSignal( &handler, SIGRTMIN + 1);

	return 1;
};



void ReceiveRequestBlockSequenceDoneSignal(){
	transferDoneFlag = true;
	puts("RequestBlock Done");
};
