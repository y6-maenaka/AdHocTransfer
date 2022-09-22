#include "ControlConnection.h"

int Sock;
char AESKey[ AES_KEY_SIZE ];
PeerInformation PeerInf;

int ClientConnection(char *servIP, unsigned short servPort){

	struct sockaddr_in servAddr;
	struct sigaction handler;
	//PeerInformation PeerInf;

	if (( Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ConnectionErrorHandling("socket creating failure");


	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(servPort);  // host to network short


	// IPAddrから汎用Addrにキャスト
	if( connect(Sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		ConnectionErrorHandling("connect failure");

	printf("Client | Socket Generated -> %d\n", Sock);

	PeerInf.TCPPeerSock = Sock;

	SetNonBlocking(Sock);

	SetSIGIO(&handler);

	return Sock;
}



int ServerConnection(unsigned short servPort){

	int servSock;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	struct sigaction handler;
	//PeerInformation PeerInf;

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
	if (( Sock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0)
		ConnectionErrorHandling("accept failure");

	printf("Server | Socket Generated -> %d\n", Sock);

	PeerInf.TCPPeerSock = Sock;
	memcpy(PeerInf.PeerIP, &(clntAddr.sin_addr.s_addr), 4);

	// setting signal(SIGIO) =======================

	SetNonBlocking(Sock);

	SetSIGIO(&handler);

	//SetNonBlocking(Sock);

	return Sock;
}


void SetSIGIO(struct sigaction *handler){

	handler->sa_handler = ReceiveCommand;
	handler->sa_flags = 0;

	// 全てのシグナルをマスク
	if ( sigfillset( &(handler->sa_mask) ) < 0)
		ConnectionErrorHandling("sigfillset failure");

	// キーボード割り込みを許可
	if ( sigdelset( &(handler->sa_mask), SIGINT ) < 0 )
		ConnectionErrorHandling("sigdelset failure");

	// KILLを許可
	if ( sigdelset( &(handler->sa_mask), SIGKILL) < 0 )
		ConnectionErrorHandling("sigdelset failure");

	// デフォルトのシグナル設定をオーバーライド
	if ( sigaction(SIGIO, handler, 0) < 0)
		ConnectionErrorHandling("sigaction failure");

	printf("===== Override signal settings =====\n");
}

void SetNonBlocking(int sock){

	// シグナルの送信先をこのプロセスにする
	if ( fcntl( sock, F_SETOWN, getpid()) < 0 )
		ConnectionErrorHandling("fcntl failure");

	// ノンブロッキング設定
	if ( fcntl( sock, F_SETFL, O_NONBLOCK | FASYNC) < 0 )
		ConnectionErrorHandling("fcntl failure");

	printf("===== Non-Block is set in the socket =====\n");
}



void ConnectionErrorHandling(char *errorMessage){
	perror(errorMessage);
	exit(1);
}


void SIGIOHandler(int signalType){
	printf("signal detected\n");
}


void SendCommand( int sock ,int command, const size_t fileSize, void *file){

	int sendedSize;
	size_t formatCommandSize;
	ssize_t sendSize;

	void *formatCommand;
	formatCommandSize = sizeof(char) + sizeof(int) + sizeof(size_t);
	formatCommand = malloc( formatCommandSize );

	memcpy(formatCommand, "$", sizeof(char));
	memcpy(formatCommand + 1, &command, sizeof(int));
	memcpy(formatCommand + 1 + sizeof(int), &fileSize, sizeof(size_t));

	void *packageCommand;
	packageCommand = malloc( formatCommandSize + fileSize );

	memcpy( packageCommand, formatCommand, formatCommandSize);
	memcpy( packageCommand + formatCommandSize, file, fileSize);

	//EVP_PKEY *pkey = NULL;
	//void *tmp;
	//tmp = malloc(8);
	//memcpy(&tmp, packageCommand + formatCommandSize, 8);
	//EVP_PKEY_print_public_fp(stdout, pkey, 0, NULL);
	//EVP_PKEY_print_public_fp(stdout, (EVP_PKEY *)tmp, 0, NULL);

	sendSize = send(sock, packageCommand, formatCommandSize + fileSize, 0);

	printf("===== Send Package Command ( %zu bytes ) =====\n", sendSize);

	free(formatCommand);
	free(packageCommand);

}


void GetSocketQSize(int sock, int *SendQSize, int *RecvQSize){

	unsigned int socketQOptLen = sizeof(SendQSize);

	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, SendQSize , &socketQOptLen );
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, RecvQSize , &socketQOptLen );

};


void SetSocketQSize(int sock, int SendQSize, int RecvQSize){	
	
	unsigned int socketQOptLen = sizeof( SendQSize );

	if ( SendQSize ){
		setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &SendQSize , socketQOptLen);
		puts("koko desu");
	}

	if ( RecvQSize )
		setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &RecvQSize , socketQOptLen);

};



void ReceiveCommand(){

	char buf[1] = {0};
	unsigned char controlMessage[ COMMAND_LENGTH + FILE_SIZE_LENGTH ];
	int command;
	size_t fileSize;
	void *file;
	ssize_t recvSize = 0;

	// ===== selectの設定 =====

	struct timeval selTimeout;
	fd_set sockSet;
	int maxDescriptor = Sock + 1;

	FD_ZERO(&sockSet);
	FD_SET(Sock, &sockSet);
	selTimeout.tv_sec = 0;
	selTimeout.tv_usec = 0;

	// =======================

	if ( select(maxDescriptor, &sockSet, NULL, NULL, &selTimeout) > 0 ){

		for(;;){

			recvSize = recv(Sock, buf, 1, 0);

			if ( errno == EWOULDBLOCK || recvSize <= 0){
				printf("brake with EWOULDBLOCK\n");
				break;
			}

			if ( buf[0] == '$' ){

				recvSize = recv( Sock, controlMessage, sizeof(controlMessage) , 0);

				if( errno == EWOULDBLOCK  /* EAGAIN */ || recvSize <= 0 ){
					break;
					// SIGALRMが送信される(or error)とrecvは-1を返す。
				 }

				memcpy( &command, controlMessage, sizeof(command));
				memcpy( &fileSize, controlMessage + sizeof(command), sizeof(fileSize));

				//printf(" command is -> %d\n", command);
				//printf(" file size is -> %zu\n", fileSize);

				file = NULL;

				printf("file size is -> %zu\n", fileSize);

				if( fileSize != 0){
					file = malloc(fileSize);
					recvSize = recv( Sock , file, fileSize, 0);
				}

				HandleCommand( command, fileSize, file);

			}
		}
	}
	// callocをfreeする

};


void HandleCommand(const int controlMessage, const size_t fileSize, void *file){
	
	
	switch(controlMessage){
		int QSize;

		case RSA_PUBLIC_KEY:
			break;

		case SOCKET_RECEIVEQ_SIZE:
			memcpy( &QSize, file, fileSize);
			printf("Get SocketQ Size -> %d\n", QSize);
			break;

		case SOCKET_SENDQ_SIZE:
			memcpy( &QSize, file, fileSize);
			printf("Get SocketQ Size -> %d\n", QSize);
			break;
	}

};




void ReceiveCommandTimeOut(){
	printf("ControlMessage empty");
};
