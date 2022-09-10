#include "ConvertFile.h"
#include "HashCipher.h"
#include "ControlFile.h"


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


	// Generate Upload File Path
	sprintf(uploadFilePath,"%s%s", UPLOAD_FOLDER_PATH,uploadFileName);

	FILE *upload_fp = fopen( uploadFilePath, "rb" );
	if(upload_fp == NULL){
		fprintf(stderr, "can not upload file");
		exit(1);
	}

	fileSize = GetFileSize( upload_fp );
	key.blockSize = fileSize;

	wholeDigestMessageNum = (int)ceil( (double)fileSize / BLOCKSIZE );
	// EVP_MAX_MD_SIZE = SHA512 hash size
	_wholeDigestMessage = malloc( wholeDigestMessageNum * EVP_MAX_MD_SIZE );


	// Generate temporary file name
	uuid_string_t _convertingFileName;
	GenerateUUIDString(_convertingFileName);
	char convertingFileName[BLOCK_FOLDER_PATH_SIZE + 1 + 1 + sizeof(_convertingFileName) + BLOCK_EXTENSION_SIZE ];
	sprintf(convertingFileName,"%s.%%%s%s",BLOCK_FOLDER_PATH, _convertingFileName, BLOCK_EXTENSION);


	FILE *block_fp = fopen(convertingFileName, "wb");
	if ( block_fp == NULL ){
		fprintf(stderr,"can not open new block file");
		exit(1);
	}


	int counter = 0;
	for(;;){
		if ( (_readedSize = fread( block.bodyContent, 1, BLOCKSIZE, upload_fp)) < BLOCKSIZE ){

			WriteToBlockFile(block, _readedSize, block_fp, _digestMessage);
			memcpy(_wholeDigestMessage + (counter * EVP_MAX_MD_SIZE), _digestMessage , EVP_MAX_MD_SIZE);
			break;

		}

		WriteToBlockFile(block, _readedSize, block_fp, _digestMessage);
		memcpy(_wholeDigestMessage + (counter * EVP_MAX_MD_SIZE), _digestMessage, EVP_MAX_MD_SIZE);

		counter += 1;
	}

	sha512Hash( (char *)_wholeDigestMessage, hashedWholeDigestMessage, (wholeDigestMessageNum * EVP_MAX_MD_SIZE) );

	free(_wholeDigestMessage);

	CreateKeyFile( hashedWholeDigestMessage, key, uploadFileName, _convertingFileName, sizeof(key) , fileSize);

	ConvertFileToBlockDone(convertingFileName, _convertingFileName, sizeof(_convertingFileName));


	fclose(upload_fp);
	fclose(block_fp);

};


void BlockToFile(char *blockFileName){
	FILE *block_fp = fopen("BlockFolder/%42FDC580-EA82-4867-8B76-BFF1506085BD.ablock", "rb");
	FILE *download_fp = fopen("DownloadFolder/sample_video.mp4", "wb");
	Block block;

	if ( block_fp == NULL || download_fp == NULL ){
		fprintf(stderr,"can not open file");
		exit(1);
	}

	size_t _readedSize;
	char readedBlock[BLOCKSIZE] = {0};
	unsigned char readedDigestMessage[EVP_MAX_MD_SIZE] = {0};

	size_t _readedDigestMessageSize, _readedBlockSize;

	int tmp = 0;

	int counter = 0;
	for(;;){
		_readedDigestMessageSize = fread( block.digestMessage, 1, EVP_MAX_MD_SIZE, block_fp );

		if (( _readedBlockSize = fread( block.bodyContent, 1, BLOCKSIZE, block_fp ) < BLOCKSIZE )){
			CheckBlock( block, _readedBlockSize );
			fwrite( block.bodyContent, 1, _readedBlockSize, download_fp );
			break;
		}

		tmp = CheckBlock( block , _readedBlockSize );
		printf("%d",tmp);
		fwrite( block.bodyContent, 1, BLOCKSIZE, download_fp );
		counter += 1;
	}

	fclose( block_fp );
	fclose( download_fp );

};



void CreateKeyFile(unsigned char *hashedWholeDigestMessage, BlockKey key, char *uploadFileName, char *convertingFileName, size_t keySize, unsigned int blockSize){

	char keyFileName[ KEY_FOLDER_PATH_SIZE + 255 + KEY_EXTENSION_SIZE ];

	sprintf(keyFileName, "%s%s%s",KEY_FOLDER_PATH, uploadFileName, KEY_EXTENSION);

	FILE *key_fp = fopen(keyFileName, "wb");

	memcpy( key.blockDigestMessage, hashedWholeDigestMessage, EVP_MAX_MD_SIZE );
	memcpy( key.fileName, uploadFileName, strlen(uploadFileName) );
	memcpy( key.blockName, convertingFileName, 37);

	time(&key.createdAt);
	time(&key.lastAccess);

	fwrite( &key, keySize, 1, key_fp);

	fclose(key_fp);
};



void WriteToBlockFile(Block block, size_t readedSize, FILE *block_fp, unsigned char * digestMessage){

	sha512Hash( block.bodyContent, block.digestMessage, readedSize);
	memcpy(digestMessage , block.digestMessage, EVP_MAX_MD_SIZE);

	fwrite( block.digestMessage, 1, EVP_MAX_MD_SIZE, block_fp );
	fwrite( block.bodyContent, 1, readedSize, block_fp );

};


int CheckBlock( Block block , size_t readedBlockSize ){

	unsigned char *digestMessage;

	sha512Hash( block.bodyContent, digestMessage, readedBlockSize );

	if ( memcmp(digestMessage, block.digestMessage, EVP_MAX_MD_SIZE ) != 0 )
		return 1;
	else
		return 0;
};


void ConvertFileToBlockDone(char *convertingFileName, char *_convertingFileName, size_t fileNameSize){
	char blockFileName[ BLOCK_FOLDER_PATH_SIZE + 1 + fileNameSize + BLOCK_EXTENSION_SIZE ];
	sprintf(blockFileName,"%s%%%s%s",BLOCK_FOLDER_PATH, _convertingFileName, BLOCK_EXTENSION);
	rename(convertingFileName, blockFileName);
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

	char AESKey[16] = "abcdefghijklmnop";
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

	char AESKey[16] = "abcdefghijklmnop";

	char *decryptedFileData;
	decryptedFileData = malloc( fileSize );

	AESDecrypt( AESKey, fileData, fileSize, NULL, decryptedFileData );

	free( fileData );

	file_fp = fopen( filePath, "wb");

	fwrite( decryptedFileData , fileSize, 1, file_fp );

	free( decryptedFileData );
	fclose( file_fp );
};
