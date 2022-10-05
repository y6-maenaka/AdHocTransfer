#include "header/HashCipher.h"
#include "header/ControlFile.h"
#include "header/ControlConnection.h"
#include "header/ConvertFile.h"
#include "header/TransferData.h"

#define SIGRTMIN 33

void *TransferThread( PeerInformation *PeerInf  );

int AllowRequestBlockDoneSignal();

void ReceiveRequestBlockSequenceDoneSignal();



int main(int argc, char *argv[]){

	LoadConfig();

	//ServerConnection();
	//ReceiveBlock();
	
	unsigned short servPort = atoi( argv[1]);
	
	ServerConnection( servPort );



	struct ThreadArgs threadArgs;
	threadArgs.PeerInf = PeerInf;
	threadArgs.mainThreadID = pthread_self();
	// スレッドを作成する前にUDPポートを確認しておく
	//pthread_t threadID;
	//pthread_create( &threadID, NULL, TransferThrea, (void *)threadArgs);

	sleep(10);
};



void *TransferThread( struct ThreadArgs threadArgs ){
	pthread_detach( pthread_self());

	PeerInf->PeerUDPSock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP); // socketの作成
																																		
	// ======= ノンブロッキング設定 =======
	struct sigaction handler;
	handler.sa_handler = ReceiveBlock;
	SetNonBlocking( PeerInf->UDPPeerSock );
	SetSignal( &handler, SIGIO );

	RequestBlock( PeerInf );

	pthread_kill( threadArgs.mainThreadID, SIGRTMIN+1);


	return (NULL);
};


int AllowRequestBlockDoneSignal(){
	struct sigactino handler;
	handler.sa_handler = ReceiveRequestBlockSequenceDoneSignal;
	SetSignal( &handler, SIGRTMIN + 1);

	return 1;
};




void *ReceiveRequestBlockSequenceDoneSignal(){
	puts("RequestBlock Done");
};
