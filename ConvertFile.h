#ifndef _H_ConvertFile_
#define _H_ConvertFile_

#include <stdio.h>
#include <string.h>
#include <uuid/uuid.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <errno.h>

#define SHA256SIZE 32
#define BLOCKSIZE 64016

#define SOCKET_BASE_RECEIVEQ_SIZE 512960
#define SOCKET_BASE_SENDQ_SIZE 512960
#define BLOCK_FLAGS_SIZE 4200

#define UPLOAD_FOLDER_PATH "UploadFolder/"
#define DOWNLOAD_FOLDER_PATH "DownloadFolder/"
#define BLOCK_FOLDER_PATH "BlockFolder/"
#define BLOCK_KEY_FOLDER_PATH "KeyFolder/"

#define BLOCK_EXTENSION ".ablock"
#define BLOCK_KEY_EXTENSION ".akey"

#define BLOCK_FOLDER_PATH_SIZE 12 // n(BlockFolder/)
#define BLOCK_EXTENSION_SIZE 7 // n(.ablock)

#define BLOCK_KEY_FOLDER_PATH_SIZE  10 // n(KeyFolder/)
#define BLOCK_KEY_EXTENSION_SIZE 5 // n(.akey)

#define UPLOAD_FOLDER_PATH_SIZE 13 // n(UploadFolder/)

#define BLOCK_NAME_LENGTH 128 + 1


typedef struct BLOCK{
	char bodyContent[ BLOCKSIZE ];
	unsigned char digestMessage[64];
}Block;


typedef struct BLOCKKEY{
	unsigned int originalFileSize;
	unsigned int blockNum;
	char fileName[255];
	char blockName[37];
	unsigned char blockFlags[BLOCK_FLAGS_SIZE];
	unsigned char blockDigestMessage[64];
	time_t createdAt;
	time_t lastAccess;
	//void *blockFlags;
	// bitMap
}BlockKey;



void FileToBlock(char *uploadFileName);
void BlockToFile(char *blockFileName);

void CreateKeyFile(unsigned char *hashedwholeDigestMessage, BlockKey key, char *uploadFileName, char *temporaryFileName, size_t keySize, unsigned int blockSize);

void WriteToBlockFile(Block block, size_t readedSize, FILE *block_fp, unsigned char *digestMessage, void *blockFlags, int counter);

int CheckBlock( Block block, size_t readedBlockSize );

void ConvertFileToBlockDone( char *temporaryName, int counter, char *stringDigestMessage);

void AESEncryptFile(char *filePath);
void AESDecryptFile(char *filePath);

//void FormatBlockKey( void *blockKeyBuffer, BlockKey *key);
BlockKey FormatBlockKey( void *blockKeyBuffer );

void CreateStringDigest( unsigned char *digest,char *stringDigest ); // 16進数→文字列 不適コード


#endif // _H_ConvertFile_
