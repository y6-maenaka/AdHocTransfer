#include "ConvertFile.h"
#include "HashCipher.h"
#include "ControlFile.h"


char AESKey[AES_KEY_SIZE];


void FileToBlock(char *uploadFileName){

	char readBuffer[BLOCKSIZE];
	unsigned char _digestMessage[EVP_MAX_MD_SIZE] = {0};
	char uploadFilePath[ UPLOAD_FOLDER_PATH_SIZE + 255 ];
	size_t _readedSize;
	long int fileSize;
	unsigned int wholeDigestMessageNum;
	unsigned char* _wholeDigestMessage;
	unsigned char hashedWholeDigestMessage[EVP_MAX_MD_SIZE];

	Block block = {0};
	BlockKey key = {0};
	//key.blockFlags = calloc( BLOCK_FLAGS_SIZE, 1);


	// Generate Upload File Path
	sprintf(uploadFilePath,"%s%s", UPLOAD_FOLDER_PATH, uploadFileName);

	FILE *upload_fp = fopen( uploadFilePath, "rb" );
	if(upload_fp == NULL){
		fprintf(stderr, "can not upload file");
		exit(1);
	}

	fileSize = GetFileSize( upload_fp );
	key.originalFileSize = fileSize;

	wholeDigestMessageNum = (int)ceil( (double)fileSize / BLOCKSIZE );

	// 分割（block)ファイル数の書き込み
	key.blockNum = wholeDigestMessageNum;

	// EVP_MAX_MD_SIZE = SHA512 hash size
	_wholeDigestMessage = malloc( wholeDigestMessageNum * EVP_MAX_MD_SIZE );

	char temporaryName[37];
	GenerateUUIDString(temporaryName);
	temporaryName[36] = '\0';

	char temporaryFileName[ BLOCK_FOLDER_PATH_SIZE + 1 + sizeof(temporaryName) + sizeof(char) + sizeof(int) + sizeof(char) + sizeof(BLOCK_EXTENSION) ];
	FILE *block_fp = NULL;

	int counter = 0;
	for(;;){
		sprintf(temporaryFileName, "%s%%%s(%d)%s", BLOCK_FOLDER_PATH, temporaryName, counter + 1, BLOCK_EXTENSION);

		block_fp = fopen(temporaryFileName, "wb");
		if( block_fp == NULL ){
			fprintf(stderr,"can not open new block file");
			exit(1);
		}

		if ( (_readedSize = fread( block.bodyContent, 1, BLOCKSIZE, upload_fp)) < BLOCKSIZE ){
			WriteToBlockFile( block, _readedSize, block_fp, _digestMessage, key.blockFlags, counter);
			fclose( block_fp );
			memcpy(_wholeDigestMessage + (counter * EVP_MAX_MD_SIZE), _digestMessage , EVP_MAX_MD_SIZE);
			break;
		}

		WriteToBlockFile( block, _readedSize, block_fp, _digestMessage, key.blockFlags, counter);
		fclose( block_fp );
		memcpy(_wholeDigestMessage + (counter * EVP_MAX_MD_SIZE), _digestMessage, EVP_MAX_MD_SIZE);

		counter += 1;
	}

	key.blockNum = counter + 1;


	sha512Hash( (char *)_wholeDigestMessage, hashedWholeDigestMessage, (wholeDigestMessageNum * EVP_MAX_MD_SIZE) );

	free(_wholeDigestMessage);

	CreateKeyFile( hashedWholeDigestMessage, key, uploadFileName, temporaryName, sizeof(key) , fileSize);

	//ConvertFileToBlockDone(temporaryFileName, _temporaryFileName, sizeof(_temporaryFileName));

	unsigned char buf[1] = {0};	
	for(int i=0; i<525; i++){
		memcpy( buf, key.blockFlags + i, 1);
		printf("%d ", *buf);
	}

	fclose(upload_fp);
	fclose(block_fp);

};


void BlockToFile(char *blockFileName){

	Block block;
	BlockKey key;
	FILE *blockKey_fp = NULL;
	unsigned char blockKeyBuffer[sizeof( BlockKey )] = {0}; // ☆

	blockKey_fp = fopen("KeyFolder/sample_video.mp4.akey", "rb");
	fread( blockKeyBuffer, sizeof(key), 1, blockKey_fp);
	fclose(blockKey_fp);

	FormatBlockKey( blockKeyBuffer, &key);



	FILE *block_fp = NULL;
	char blockFilePath[ BLOCK_FOLDER_PATH_SIZE + 1 + sizeof(key.blockName) + sizeof(char) + sizeof(int) + sizeof(char) + sizeof(BLOCK_EXTENSION_SIZE) + 1];

	unsigned char *readedBlock;
	size_t encryptBlockSize = GetAESEncryptedDataSize( BLOCKSIZE + EVP_MAX_MD_SIZE );
	readedBlock = malloc( encryptBlockSize );
	char *decryptBlock;
	decryptBlock = malloc( BLOCKSIZE + EVP_MAX_MD_SIZE );

	FILE *download_fp = fopen("DownloadFolder/sample_video.mp4", "wb");

	for(int i=0; i < key.blockNum; i++){
		sprintf(blockFilePath ,"%s%%%s(%d)%s",BLOCK_FOLDER_PATH, key.blockName, i+1, BLOCK_EXTENSION);

		block_fp = fopen( blockFilePath, "rb");

		if ( block_fp == NULL ){
			puts("continue with no file");
			continue;
		}

		else{
			fread( readedBlock, encryptBlockSize , 1 , block_fp);
			AESDecrypt( AESKey, readedBlock, encryptBlockSize, NULL, decryptBlock );
			memcpy( block.digestMessage , decryptBlock, EVP_MAX_MD_SIZE);
			memcpy( block.bodyContent, decryptBlock + EVP_MAX_MD_SIZE, BLOCKSIZE );

			// 最後のファイルは書き込みバイト数が異なる
			if ( i != key.blockNum - 1){
				fwrite( block.bodyContent, BLOCKSIZE, 1 ,download_fp);
			}
			else{
				fwrite( block.bodyContent, key.originalFileSize - (BLOCKSIZE * i), 1, download_fp);
			}

		}
	}

	free(readedBlock);
	free(decryptBlock);
	fclose( block_fp );
	fclose( download_fp );

};



void CreateKeyFile(unsigned char *hashedWholeDigestMessage, BlockKey key, char *uploadFileName, char *temporaryName, size_t keySize, unsigned int originalFileSize){

	char keyFileName[ BLOCK_KEY_FOLDER_PATH_SIZE + 255 + BLOCK_KEY_EXTENSION_SIZE ];

	sprintf(keyFileName, "%s%s%s",BLOCK_KEY_FOLDER_PATH, uploadFileName, BLOCK_KEY_EXTENSION);

	FILE *key_fp = fopen(keyFileName, "wb");
	if ( key_fp == NULL ){
		fprintf(stderr, " can not open the key file");
		exit(1);
	}

	memcpy( key.blockDigestMessage, hashedWholeDigestMessage, EVP_MAX_MD_SIZE );
	memcpy( key.fileName, uploadFileName, sizeof(key.fileName) );
	memcpy( key.blockName, temporaryName, 37);


	time(&key.createdAt);
	time(&key.lastAccess);

	fwrite( &key, keySize, 1, key_fp);

	fclose(key_fp);
};



void WriteToBlockFile(Block block, size_t readedSize, FILE *block_fp, unsigned char * digestMessage, void *blockFlags, int counter){

	sha512Hash( block.bodyContent, block.digestMessage, readedSize);
	memcpy(digestMessage , block.digestMessage, EVP_MAX_MD_SIZE);

	char *writeBlock;
	unsigned char *encryptWriteBlock;
	size_t encryptWriteBlockSize;
	unsigned char bitMapBuf[1];
	memset( &bitMapBuf, 0x00, 1); // なくても良い
	//bitMapBuf = calloc(1,1);

	writeBlock = calloc( EVP_MAX_MD_SIZE + BLOCKSIZE , 1);
	memcpy(writeBlock, block.digestMessage, EVP_MAX_MD_SIZE );
	memcpy(writeBlock + EVP_MAX_MD_SIZE, block.bodyContent, BLOCKSIZE );

	encryptWriteBlockSize = GetAESEncryptedDataSize( EVP_MAX_MD_SIZE + BLOCKSIZE );
	encryptWriteBlock = calloc( encryptWriteBlockSize , 1);
	AESEncrypt( AESKey, writeBlock, EVP_MAX_MD_SIZE +  BLOCKSIZE, NULL, encryptWriteBlock );

	fwrite( encryptWriteBlock, encryptWriteBlockSize , 1, block_fp );


	memcpy( &bitMapBuf, blockFlags + (counter/8) , 1);
	*bitMapBuf |= (unsigned int)(pow(2,  7 - (counter % 8) ));
	memcpy( blockFlags + (counter/8), &bitMapBuf, 1);

	printf("%d\n",*bitMapBuf);

	free(writeBlock);
	free(encryptWriteBlock);
};


int CheckBlock( Block block , size_t readedBlockSize ){

	unsigned char *digestMessage;

	sha512Hash( block.bodyContent, digestMessage, readedBlockSize );

	if ( memcmp(digestMessage, block.digestMessage, EVP_MAX_MD_SIZE ) != 0 )
		return 1;
	else
		return 0;

};


void ConvertFileToBlockDone(char *temporaryFileName, unsigned char *digestMessage, size_t fileNameSize, size_t fileNum){
	/*
	char blockFileName[ BLOCK_FOLDER_PATH_SIZE + 1 + sizeof(temporaryFileName) + sizeof(char) + sizeof(int) + sizeof(char) + sizeof(BLOCK_EXTENSION)];
	char digestFileName[EVP_MAX_MD_SIZE + 1];
	memcpy(digestFileName, digestMessage, EVP_MAX_MD_SIZE);
	digestFileName[EVP_MAX_MD_SIZE] = '\0';
	char finalyFileName[ BLOCK_FOLDER_PATH_SIZE + 1 + sizeof(digestFileName) + sizeof(char) + sizeof(int) + sizeof(char) + sizeof(BLOCK_EXTENSION)];
	for(int i=1; i < fileNum; i++){
		sprintf(temporaryFileName, "%s%%%s(%d)%s", BLOCK_FOLDER_PATH, temporaryName, i, BLOCK_EXTENSION);
		sprintf();
		rename(temporaryFileName, finalyFileName);
	}
	*/
};


void AESEncryptFile(char *filePath){

	FILE *file_fp = fopen(filePath,"rb");

	if ( file_fp == NULL){
		fprintf(stderr,"can not open the file");
		exit(1);
	}

	unsigned char *fileData;

	long int fileSize;
	fileSize = GetFileSize(file_fp);

	fileData = malloc(fileSize);

	if (!fileData){
		fprintf(stderr,"can not get enought memory size to encrypt file");
		exit(1);
	}

	fread( fileData, fileSize, 1, file_fp );

	fclose(file_fp);

	//char AESKey[16] = "abcdefghijklmnop";
	unsigned int encryptedDataSize = GetAESEncryptedDataSize(fileSize);

	unsigned char *encryptedFileData;
	encryptedFileData = malloc( encryptedDataSize );

	AESEncrypt( AESKey, (char *)fileData , fileSize, NULL, encryptedFileData);

	free( fileData );

	file_fp = fopen(filePath, "wb");

	fwrite( encryptedFileData, encryptedDataSize, 1, file_fp );


	free(encryptedFileData);
	fclose(file_fp);

};

void AESDecryptFile(char *filePath){
	FILE *file_fp = fopen(filePath, "rb");

	if ( !file_fp ){
		fprintf(stderr,"can not open the file");
		exit(1);
	}

	unsigned char *fileData;

	long int fileSize;
	unsigned int encryptedDataSize;
	fileSize = GetFileSize(file_fp);

	fileData = malloc(fileSize);

	if ( !fileData ){
		fprintf(stderr,"can not get enought memory size to decrypt file");
		exit(1);
	}

	fread( fileData, fileSize, 1, file_fp );

	fclose( file_fp );

	//char AESKey[16] = "abcdefghijklmnop";

	char *decryptedFileData;
	decryptedFileData = malloc( fileSize );

	AESDecrypt( AESKey, fileData, fileSize, NULL, decryptedFileData );

	free( fileData );

	file_fp = fopen( filePath, "wb");

	fwrite( decryptedFileData , fileSize, 1, file_fp );

	free( decryptedFileData );
	fclose( file_fp );
};



void FormatBlockKey( void *blockKeyBuffer, BlockKey *key){

	memcpy( &(key->originalFileSize), blockKeyBuffer, sizeof(key->originalFileSize) );
	memcpy( &(key->blockNum), blockKeyBuffer + sizeof(key->originalFileSize) , sizeof(key->blockNum));
	memcpy( key->fileName, blockKeyBuffer + sizeof(key->originalFileSize) + sizeof(key->blockNum), sizeof(key->fileName));

	memcpy( key->blockName, blockKeyBuffer + sizeof(key->originalFileSize) + sizeof(key->blockNum) + sizeof(key->fileName), sizeof(key->blockName));

  memcpy( key->blockFlags, blockKeyBuffer + sizeof(key->originalFileSize) + sizeof(key->blockNum) + sizeof(key->fileName) + sizeof(key->blockName), sizeof(key->blockFlags));


};
