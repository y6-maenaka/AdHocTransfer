#include "header/HashCipher.h"
#include "header/ControlFile.h"
#include "header/ControlConnection.h"
#include "header/ConvertFile.h"
#include "header/TransferData.h"

#include <pthread.h>
#include <unistd.h>

#define SIGRTMIN 33


struct ThreadArgs{
	PeerInformation PeerInf;
	pthread_t mainThreadID;
};


void *TransferThread( struct ThreadArgs threadArgs );

void ReceiveTransferDoneSignal();

int AllowTransferDoneSignal();





int main( int argc, char *argv[]){

	PeerInformation *PeerInf = GetPeerInformation();
	LoadConfig();

	char *servIP;
	unsigned short servPort;
	
	servIP = argv[1];
	servPort = atoi( argv[2] );

	//ClientConnection( &servIP, servPort ); // and PeerInfの生成
	
	//BindUDPPort();
	//SendCommand( PeerInf.TCPPeerSock, UDP_PORT, sizeof(int), &PeerInf.UDPPeerPort);
	
	PeerInf->TCPPeerSock = ClientConnection( servIP, servPort );

	// -----ノンブロッキング設定--------- 
	//struct sigaction handler;
	//handler.sa_handler = ReceiveRequest;

	//PeerInf->PeerUDPPort = BindUDPPort( PeerInf );
	//SetNonBlocking( PeerInf->UDPPeerSock );
	//SetSignal( &handler, SIGIO );
	// ------------------------

	sleep(2);
	SendCommand( PeerInf->TCPPeerSock, UDP_PORT, sizeof(PeerInf->PeerUDPPort), &PeerInf->PeerUDPPort );


	// === TransferThreadの起動=================
	struct ThreadArgs threadArgs;
	threadArgs.PeerInf = *PeerInf;
	threadArgs.mainThreadID = pthread_self();
	//pthread_t threadID;		
	//pthread_create(%threadID, NULL, TransferThread, (void *)threadArgs);
	
	// ===========================================

	sleep( 5 );

};




void *TransferThread( struct ThreadArgs threadArgs ){
	pthread_detach( pthread_self()); // デタッチモード-> スレッドが終了したらリソースを解放する
																	 
	BindUDPPort();
	// ---- ノンブロッキング設定 ------------
	struct sigaction handler;
	handler.sa_handler = ReceiveRequest;
	SetNonBlocking( threadArgs.PeerInf.UDPPeerSock );
	SetSignal( &handler , SIGIO);
	// ---------------------------


	SendCommand( threadArgs.PeerInf.TCPPeerSock, UDP_PORT, sizeof(threadArgs.PeerInf.PeerUDPPort), &threadArgs.PeerInf.PeerUDPPort );
	// バインドしたポートを解放しない
	//
	// ファイルが全て集まったら、UDPPortを解放する
	//close( PeerInf.UDPPeerSock );
	
	pthread_kill( threadArgs.mainThreadID, SIGRTMIN+1);
	
	return (NULL);
};




int AllowTransferDoneSignal(){

	struct sigaction handler;
	handler.sa_handler = ReceiveTransferDoneSignal;
	SetSignal( &handler, SIGRTMIN + 1);
	// SetSignalに失敗したら0を返す
	
	return 1;	
};




void ReceiveTransferDoneSignal(){
	puts("Done TransferData");
};
