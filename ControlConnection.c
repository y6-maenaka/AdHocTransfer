#include "ControlConnection.h"


int ClientConnection(char *servIP, unsigned short servPort){

	int sock;
	struct sockaddr_in servAddr;
	struct sigaction handler;
	PeerInformation PeerInf;

	if (( sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ConnectionErrorHandling("socket creating failure");

	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(servPort);  // host to network short

	// IPAddrから汎用Addrにキャスト
	if( connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		ConnectionErrorHandling("connect failure");

	PeerInf.TCPPeerSock = sock;

	// 非同期I/O設定 =======================================

	/*
		struct sigaction{
			void (*sa_handler)(int) // シグナルハンドラ
			sigset_t sa_mask; // マスク(block)するシグナル sigfillset() etc..
			int sa_flags; // フラグ
			....
			....
		}
		※ sa_handlerフィールドは,int型のパラメータを１つとってvoidを返す関数へのポインタ
		SIG_DEF, SIG_IGN　て椅子の指定もできる
	 */

	handler.sa_handler = SIGIOHandler;

	// 全てのシグナルをマスクする
	if (sigfillset(&handler.sa_mask) < 0)
		ConnectionErrorHandling("sigfillset failure");

	handler.sa_flags = 0;

	// デフォルトのシグナル設定のオーバーライド(sigaction())
	if (sigaction(SIGIO, &handler, 0) < 0)
		ConnectionErrorHandling("sigaction failure");

	/*
	 function control() : [1:ディスクリプタ],[2:取得or変更].[3:Flag(O_NONBLOCK)]
	 socketもディスクリプタで制御可
	 標準入出力も制御可
	 */

	// シグナルの送信先をこのプロセスにする
	if (fcntl(sock, F_SETOWN, getpid()) < 0)
		ConnectionErrorHandling("fcntl failure");

	// シグナル
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
		ConnectionErrorHandling("fcntl failure");

	// ===========================================================

	close(sock);

	return sock;
}



int ServerConnection(unsigned short servPort){

	int servSock , clntSock;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	struct sigaction handler;
	PeerInformation PeerInf;

	if (( servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ConnectionErrorHandling("socket creating failure");

	memset(&servAddr, 0x00, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr
	servAddr.sin_port = htons(servPort);

	if ( bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		ConnectionErrorHandling("bind failure");

	if (listen(servSock, MAXPENDING) < 0)
		ConnectionErrorHandling("listen failure");

	unsigned int clntLen = sizeof(clntAddr);
	// 許可したらsocketを複製
	if (( clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0)
		ConnectionErrorHandling("accept failure");

	PeerInf.TCPPeerSock = clntSock;
	memcpy(PeerInf.PeerIP, &(clntAddr.sin_addr.s_addr), 4);

	// setting signal(SIGIO) =======================

	handler.sa_handler = SIGIOHandler;
	handler.sa_flags = 0;

	if ( sigfillset(&handler.sa_mask) < 0 )
		ConnectionErrorHandling("sigfillset failure");

	if ( sigaction(SIGIO, &handler, 0) < 0 )
		ConnectionErrorHandling("sigaction failure");

	// =============================================

	if ( fcntl(clntSock, F_SETOWN, getpid() < 0) )
		ConnectionErrorHandling("fcntl failure");

	if ( fcntl(clntSock, F_SETFL, O_NONBLOCK | FASYNC) < 0 )
		ConnectionErrorHandling("fcntl failure");

	close(clntSock);
	close(servSock);

	return clntSock;

}



void ConnectionErrorHandling(char *errorMessage){
	perror(errorMessage);
	exit(1);
}


void SIGIOHandler(int signalType){
	printf("signal detected");
}

void SendCommand(){
	printf("here is send command function");
}


void GetSocketQSize(int sock, int *SendQSize, int *RecvQSize){
	
	unsigned int sendQOptLen = sizeof(SendQSize);
	unsigned int recvQOptLen = sizeof(RecvQSize);
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, SendQSize , &sendQOptLen );
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, RecvQSize , &recvQOptLen );

};



void ReceiveCommand(int sock){
	
	char buf[1];
	unsigned char controlMessage[ COMMAND_LENGTH + FILE_SIZE_LENGTH ];
	int command;
	size_t fileSize;
	unsigned char *file;
	struct sigaction TimeOutSIG;

	TimeOutSIG.sa_handler = ReceiveCommandTimeOut;
	// 全てマスクする
	if ( sigfillset( &TimeOutSIG.sa_mask) < 0 )
		ConnectionErrorHandling(" sigfillset() failure");

	// シグナル設定
	if ( sigaction( SIGALRM, &TimeOutSIG, 0) < 0 )
		ConnectionErrorHandling(" sigaction() failure");

	for(;;){

		alarm(TIMEOUT_SECS); // timeout設定
		recv(sock, buf, 1, 0); // timeout設定
															// timeout -> break;
		if ( *buf == 37 ){
			recv( sock, controlMessage, sizeof(controlMessage) , 0);
			if( errno == EINTR )
				// SIGALRMが送信される(or error)とrecvは-1を返す。
				break;

			memcpy(&command, controlMessage, sizeof(command));
			memcpy(&fileSize, controlMessage + sizeof(command), sizeof(fileSize));

			if( fileSize != 0){
				file = calloc( 1, fileSize);
				recv( sock , file, fileSize, 0);
			}

			file = NULL;

			HandleCommand( command, fileSize, file);
		}
	}

	// callocをfreeする
	
};


void HandleCommand(const int controlMessage, const int fileSize, unsigned char *file){};


void ReceiveCommandTimeOut(){
	printf("ControlMessage empty");
};
