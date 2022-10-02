#include "header/HashCipher.h"
#include "header/ControlFile.h"
#include "header/ControlConnection.h"
#include "header/ConvertFile.h"
#include "header/TransferData.h"

int main( int argc, char *argv[]){
	LoadConfig();

	char *servIP;
	unsigned short servPort;
	
	servIP = argv[0];
	servPort = atoi( argv[1] );

	//ClientConnection( &servIP, servPort ); // and PeerInfの生成
	
	BindUDPPort();
	//SendCommand( PeerInf.TCPPeerSock, UDP_PORT, sizeof(int), &PeerInf.UDPPeerPort);

	


};
