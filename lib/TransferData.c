#include "../header/TransferData.h"
#define CommandPadding 2


unsigned char receiveBlockFlags[ BLOCK_FLAGS_SIZE ];
unsigned char notExistBlockFlags[BLOCK_FLAGS_SIZE];
struct sockaddr_in servAddr;
struct sockaddr_in clntAddr;



unsigned short BindUDPPort( PeerInformation *PeerInf){ // 送信側
									 
	//struct sigaction handler;
	
	// ポート番号をランダムで選定する
	srand( (unsigned int)time(NULL));


	if (( PeerInf->UDPPeerSock = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 )
		TransferErrorHandling("socket creating failure");

	unsigned short randomPort;
	for( int i=0; i<5; i++ ){
		randomPort = (unsigned short)((rand() % 16383) + 49512); //　ダイナミックポートナンバーを使う

		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(randomPort);


		if ( bind( PeerInf->UDPPeerSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
			if ( i >= 4 )
				TransferErrorHandling("bind failure");

			continue;

		}else{
			break;
		}


	};

	return randomPort;

};



void SendBlockPackage( int sock, struct sockaddr_in *clntAddr ,BlockPackage *package ){ // 送信側
	
	sendto( sock, &package, sizeof(RequestBlockCommand), 0, (struct sockaddr *)&clntAddr, sizeof(clntAddr));

};



void RequestBlock( PeerInformation *PeerInf ){ // 受信側

	BlockKey key;

	memset( receiveBlockFlags, 0x00, sizeof(receiveBlockFlags));
	memset( notExistBlockFlags, 0x00, sizeof(notExistBlockFlags));

	// ====================================
	RequestBlockCommand command;

	void *blockKeyBuffer;
	blockKeyBuffer = malloc( sizeof(BlockKey));
	FILE *key_fp = fopen( "../KeyFolder/sample_video.mp4.akey", "rb");

	fread( blockKeyBuffer, sizeof(BlockKey) , 1, key_fp);
	fclose( key_fp );
	
	//FormatBlockKey( blockKeyBuffer, &key);
	key = FormatBlockKey( blockKeyBuffer);
	free( blockKeyBuffer);

	// =======================================

	int position = 0;
	unsigned char flagBuf[1] = {0};
	char blockFileNameString[ BLOCK_FILE_NAME_LENGTH ] = {0};

	// ====================================================

	memset( &clntAddr, 0x00, sizeof(clntAddr));
	clntAddr.sin_family = AF_INET;
	clntAddr.sin_addr.s_addr = inet_addr( PeerInf->PeerIP );
	clntAddr.sin_port = htons( PeerInf->PeerUDPPort );

	// ====================================================


	CreateStringDigest( key.blockDigestMessage , blockFileNameString);

	
	unsigned char blockFlagsBuf[1] = {0};	
	unsigned char receiveBlockFlagsBuf[1] = {0};
	unsigned char notExistBlockFlagsBuf[1] = {0};
	unsigned char doneFlags[1] = {0};
	memset( doneFlags, 0x00, sizeof(unsigned char));

	for(;;){
		memcpy( blockFlagsBuf, key.blockFlags + position, 1);
		memcpy( receiveBlockFlagsBuf, receiveBlockFlags + position, 1);
		memcpy( notExistBlockFlagsBuf, notExistBlockFlags + position, 1);

		for(int i=7; i>=0;i--){
		  //printf("%d " , ((blockFlagsBuf[0]>>i)&1) ^ (( receiveBlockFlagsBuf[0]>>i)&1)  );
			doneFlags[0] &= (( ((blockFlagsBuf[0]>>i)&1) ^ (( receiveBlockFlagsBuf[0]>>i)&1) ) & ~((notExistBlockFlagsBuf[0]>>i)&1) );
			printf("%d", (int)doneFlags[0]);

		   if ( ( ((blockFlagsBuf[0]>>i)&1) ^ (( receiveBlockFlagsBuf[0]>>i)&1) ) & ~((notExistBlockFlagsBuf[0]>>i)&1) ){
				command = GenerateRequestCommand( blockFileNameString, position * 8 + ( 8 - i));
				SendRequestCommand( PeerInf->UDPPeerSock, &clntAddr, &command );

				printf("%d ", position * 8 + (8 -i));
			 }else{
					puts("false");
			 }
		}

		position += 1;

		if ( position >= key.blockNum && doneFlags[0] ) break;

		if ( position >= key.blockNum ){
			position = 0;
			memset( doneFlags, 0x00, sizeof(unsigned char));
		}
	}

};




void SendRequestCommand( int sock, struct sockaddr_in *clntAddr, RequestBlockCommand *command ){
	
	sendto( sock, &command, sizeof(RequestBlockCommand), 0, (struct sockaddr *)&clntAddr, sizeof(clntAddr));
};




void TransferErrorHandling( char *errorMessage){

	printf(" [Error] Transfer ");
	perror( errorMessage );
	exit(1);
};



void ReceiveRequest(){
	// signal handler
	
	char blockFilePath[ 128 + 1 + sizeof(BLOCK_FOLDER_PATH) + sizeof(BLOCK_EXTENSION)];
	FILE *block_fp = NULL;
	void *request;
	void *blockBuf;
	unsigned int servAddrLen = sizeof(servAddr);
	BlockPackage package;
	PeerInformation PeerInf;

	request = malloc( sizeof( RequestBlockCommand ));

	recvfrom( PeerInf.UDPPeerSock, request, sizeof( RequestBlockCommand), 0, (struct sockaddr *)&servAddr, &servAddrLen );

	RequestBlockCommand command = FormatRequestCommand( &request );

	if ( strcmp( &command.symbol[0], "#") != 0){
		printf("incorrect command received\n");
		return;
	}

	// コマンドのファイルを開いて送信
	sprintf(blockFilePath,"%s%s%s", BLOCK_FOLDER_PATH, command.blockFileName, BLOCK_EXTENSION);

	if( !(block_fp = fopen(blockFilePath, "rb")) ){

		package = GenerateBlockPackage( NOT_EXIST_BLOCK, command.blockFileName, command.fileNum , NULL);
		printf(" can not send the block, file not exist\n");

	}else{
		blockBuf = malloc( GetAESEncryptedDataSize( BLOCKSIZE + EVP_MAX_MD_SIZE ) );
		//ブロックファイルを送信する
		fread(blockBuf, BLOCKSIZE+EVP_MAX_MD_SIZE, 1 ,block_fp);
		package = GenerateBlockPackage( EXIST_BLOCK, command.blockFileName, command.fileNum, &blockBuf);

		SendBlockPackage( PeerInf.UDPPeerSock, &servAddr , &package );
		
		printf("===== Sended Block =====\n");

		free( blockBuf );
	}	
};



void ReceiveBlock(){
	// signal handler 
	
	PeerInformation PeerInf;
	void *packageBuf;
	unsigned int clntAddrLen = sizeof(clntAddr);
	BlockPackage *package;
	unsigned char notExistBlockFlagsBuf[1] = {0};
	unsigned char receiveBlockFlagsBuf[1] = {0};
	char blockFilePath[ sizeof(BLOCK_FOLDER_PATH) + BLOCK_FILE_NAME_LENGTH + 1 +sizeof(char) + sizeof(int) + sizeof(char) + sizeof(BLOCK_EXTENSION)];

	char *encryptedBlock;
	unsigned char *decryptedBlock;

	char AESKey[ AES_KEY_SIZE ];
	char PeerAESKey[ AES_KEY_SIZE ];

	clntAddrLen = sizeof( clntAddr );
	packageBuf = malloc( sizeof(BlockPackage) + GetAESEncryptedDataSize(BLOCKSIZE + EVP_MAX_MD_SIZE));


	recvfrom( PeerInf.UDPPeerSock, packageBuf, sizeof(BlockPackage) + GetAESEncryptedDataSize(BLOCKSIZE + EVP_MAX_MD_SIZE), 0, (struct sockaddr *)&clntAddr, &clntAddrLen);

	package = ( BlockPackage *)packageBuf;

	if ( package->fileType == NOT_EXIST_BLOCK ){
		memcpy( notExistBlockFlagsBuf, notExistBlockFlags + (int)ceil( (double)package->fileNum / 8), 1);
		*notExistBlockFlagsBuf |= (unsigned int)(pow(2 , 7 - (package->fileNum % 8)));
		printf("peer not had the block\n");
		// クライアントにブロックが存在しなかった場合
	
	}else{
		// ブロックが送信されてきたとき
		encryptedBlock = malloc( BLOCKSIZE + EVP_MAX_MD_SIZE);
		decryptedBlock = malloc( GetAESEncryptedDataSize(BLOCKSIZE + EVP_MAX_MD_SIZE));

		AESDecrypt( PeerAESKey, package->blockBody, BLOCKSIZE + EVP_MAX_MD_SIZE, NULL, encryptedBlock ); // 暗号化解除
		AESEncrypt( AESKey, encryptedBlock, GetAESEncryptedDataSize(BLOCKSIZE + EVP_MAX_MD_SIZE), NULL, decryptedBlock);

		sprintf( blockFilePath, "%s#%s(%d)%s", BLOCK_FOLDER_PATH, package->blockFileName, package->fileNum ,BLOCK_EXTENSION ); // ファイルを保存
		FILE *block_fp = fopen( blockFilePath ,"wb" ); 
		fwrite( decryptedBlock , GetAESEncryptedDataSize(BLOCKSIZE + EVP_MAX_MD_SIZE),1, block_fp );
		fclose( block_fp );
		
		memcpy( receiveBlockFlagsBuf, receiveBlockFlags + (int)ceil( (double)package->fileNum / 8) , 1);
		*receiveBlockFlagsBuf |= (unsigned int)(pow(2, 7 - (package->fileNum % 8)));
		printf("receive and save block\n");
	}

};



RequestBlockCommand GenerateRequestCommand( char *blockFileName, int fileNum){

	RequestBlockCommand command;
	memcpy( command.symbol, "#", sizeof(char));
	strcpy( command.blockFileName, blockFileName);
	command.fileNum = fileNum;
	
	return command;
};



RequestBlockCommand FormatRequestCommand( void *commandBuf ){

	RequestBlockCommand *command = (RequestBlockCommand *)commandBuf;
	
	return *command;
};



BlockPackage GenerateBlockPackage( int fileType ,char *blockFileName, int fileNum, void *blockBody ){

	BlockPackage package;
	memcpy( package.symbol , "#" , sizeof(char));
	package.fileType = fileType;
	strcpy( package.blockFileName, blockFileName );
	package.fileNum = fileNum;

	if( fileType == EXIST_BLOCK ){
		memcpy( package.blockBody, blockBody, GetAESEncryptedDataSize( BLOCKSIZE + EVP_MAX_MD_SIZE ));
	}else{
		package.blockBody = NULL;	
	}

	return package;
}
