#include "../header/ControlConnection.h"


int Sock;
static PeerInformation PeerInf;


int ClientConnection(char *servIP, unsigned short servPort){ // 送信側
	struct sockaddr_in servAddr;
	struct sigaction handler;

	if (( Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ConnectionErrorHandling("socket creating failure");
	
	PeerInf.PeerIP = servIP;

	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//servAddr.sin_addr.s_addr = inet_addr( servIP );
	//servAddr.sin_port = htons(servPort);  // host to network short
	servAddr.sin_port = htons(8080);



	// IPAddrから汎用Addrにキャスト
	if( connect(Sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		ConnectionErrorHandling("connect failure");

	printf("=== Connect With ( %d ) Socket ===\n", Sock);

	SetNonBlocking(Sock);

	handler.sa_handler = ReceiveCommand;
	SetSignal(&handler, SIGIO);

	return Sock;
}




int ServerConnection(unsigned short servPort){ // 受信側

	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	struct sigaction handler;
	int servSock;
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
	if (( Sock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0)
		ConnectionErrorHandling("accept failure");

	printf("Server | Socket Generated -> %d\n", Sock);

	PeerInf.TCPPeerSock = Sock;

	puts("hy");

	//memcpy(PeerInf.PeerIP, &(clntAddr.sin_addr.s_addr), 4);

	puts(" DONE ");

	// setting signal(SIGIO) =======================

	SetNonBlocking(Sock);

	puts(" here ");

	handler.sa_handler = ReceiveCommand;
	SetSignal(&handler, SIGIO);

	puts(" done here");

	return Sock;
}



void SetSignal(struct sigaction *handler, int signalType){

	//handler->sa_handler = ReceiveCommand;
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
	if ( sigaction(signalType, handler, 0) < 0)
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
	printf(" [Error] Connection \n");
	perror(errorMessage);
	exit(1);
}


void SIGIOHandler(int signalType){
	printf("signal detected\n");
}


void SendCommand( int sock ,int command, const size_t fileSize, void *file){

	//int sendedSize;
	//size_t formatCommandSize;
	ssize_t sendSize;
	ControlCommand controlCommand;

	//void *formatCommand;
	//formatCommandSize = sizeof(char) + sizeof(int) + sizeof(size_t);
	//formatCommand = malloc( formatCommandSize );

	//memcpy(formatCommand, "$", sizeof(char));
	controlCommand.command = command;
	controlCommand.fileSize = fileSize;
	//memcpy(formatCommand + 1, &command, sizeof(int));
	//memcpy(formatCommand + 1 + sizeof(int), &fileSize, sizeof(size_t));
	
	printf(" comman -> [ %d ]\n", controlCommand.command);
	printf(" fileSize -> [ %zu ]\n", controlCommand.fileSize);


	unsigned char *packageCommand;
	packageCommand = malloc( sizeof(char) + sizeof(ControlCommand) + fileSize ); // n(symbol) + sizeof(ControlCommand) + fileSize


	//memcpy( packageCommand ,"$", sizeof(char));
	packageCommand[0] = '$';
	memcpy( packageCommand + sizeof(char), &controlCommand, sizeof(ControlCommand));
	memcpy( packageCommand + sizeof(char) + sizeof(ControlCommand), file, sizeof(fileSize));

	ControlCommand *t_command = ( ControlCommand *)(packageCommand + 1);
	unsigned short *_t = ( unsigned short *)(packageCommand + 1 + sizeof(ControlCommand));
	printf("%d\n", t_command->command);
	printf("%zu\n",t_command->fileSize);
	printf("%d\n", *_t);

	printf("%s\n", packageCommand);

	//EVP_PKEY *pkey = NULL;
	//void *tmp;
	//tmp = malloc(8);
	//memcpy(&tmp, packageCommand + formatCommandSize, 8);
	//EVP_PKEY_print_public_fp(stdout, pkey, 0, NULL);
	//EVP_PKEY_print_public_fp(stdout, (EVP_PKEY *)tmp, 0, NULL);
	
	sendSize = send(sock, &packageCommand, sizeof(char) + sizeof(ControlCommand) + (size_t)fileSize, 0);

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
	
	puts(" in receive command");
	printf(" %d ", Sock);
	
	char buf[1] = {0};
	unsigned char *commandBuf;
	//int command;
	//size_t fileSize;
	unsigned char *file;
	ssize_t recvSize = 0;
	ControlCommand *controlCommand;

	// ===== selectの設定 =====

	struct timeval selTimeout;
	fd_set sockSet;
	int maxDescriptor = Sock + 1;

	FD_ZERO(&sockSet);
	FD_SET(Sock, &sockSet);
	selTimeout.tv_sec = 0;
	selTimeout.tv_usec = 0;

	// =======================
	puts(" i'm in here");

	if ( select(maxDescriptor, &sockSet, NULL, NULL, &selTimeout) > 0 ){
		puts(" jajajaj ");
		commandBuf = malloc( sizeof(ControlCommand) );

		for(;;){

			recvSize = recv(servSock, buf, 1, 0);

			printf(" --- > %c\n", buf[0]);

			if ( errno == EWOULDBLOCK || recvSize <= 0){
				printf("brake with EWOULDBLOCK\n");
				break;
			}

			if ( buf[0] == '$' ){

				recvSize = recv( Sock, commandBuf, sizeof(ControlCommand) , 0);
				controlCommand = ( ControlCommand *)commandBuf;
				printf(" - - - - - > %d\n", controlCommand->command);

				if( errno == EWOULDBLOCK  /* EAGAIN */ || recvSize <= 0 ){
					break;
					// SIGALRMが送信される(or error)とrecvは-1を返す。
				 }

				//memcpy( &command, controlMessage, sizeof(command));
				//memcpy( &fileSize, controlMessage + sizeof(command), sizeof(fileSize));

				//printf(" command is -> %d\n", command);
				//printf(" file size is -> %zu\n", fileSize);

				file = NULL;

				printf("file size is -> %zu\n", controlCommand->fileSize);
				printf("command is -> %d\n" , controlCommand->command);

				if( controlCommand->fileSize != 0){
					file = malloc(controlCommand->fileSize);
					memset( file, 0x00, sizeof(controlCommand->fileSize));
					recvSize = recv( Sock , file, controlCommand->fileSize, 0);

					EVP_PKEY_print_public_fp( stdout, (EVP_PKEY *)file, 0, NULL);
				}

				HandleCommand( controlCommand->command, controlCommand->fileSize, file);
				free(commandBuf);

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

		case UDP_PORT:
			break;
	}

};




void ReceiveCommandTimeOut(){
	printf("ControlMessage empty");
};



PeerInformation *GetPeerInformation(){
	return &PeerInf;
};
