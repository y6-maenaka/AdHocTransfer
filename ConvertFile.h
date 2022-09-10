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
#define BLOCKSIZE 64512
#define BLOCKEXTENSIONSIZE 7
#define BLOCKFOLDERNAMESIZE 12

#define UPLOAD_FOLDER_PATH "UploadFolder/"
#define DOWNLOAD_FOLDER_PATH "DownloadFolder/"
#define BLOCK_FOLDER_PATH "BlockFolder/"
#define KEY_FOLDER_PATH "KeyFolder/"

#define BLOCK_EXTENSION ".ablock"
#define KEY_EXTENSION ".akey"

#define BLOCK_FOLDER_PATH_SIZE 12 // n(BlockFolder/)
#define BLOCK_EXTENSION_SIZE 7 // n(.ablock)

#define KEY_FOLDER_PATH_SIZE  10 // n(KeyFolder/)
#define KEY_EXTENSION_SIZE 5 // n(.akey)

#define UPLOAD_FOLDER_PATH_SIZE 13 // n(UploadFolder/)

typedef struct BLOCK{
	char bodyContent[64512];
	unsigned char digestMessage[64];
}Block;



typedef struct BLOCKKEY{
	unsigned char blockDigestMessage[64];
	char fileName[255];
	char blockName[37];
	unsigned int blockSize;
	time_t createdAt;
	time_t lastAccess;
	// bitMap
}BlockKey;



void FileToBlock(char *uploadFileName);
void BlockToFile(char *blockFileName);
void CreateKeyFile(unsigned char *hashedwholeDigestMessage, BlockKey key, char *uploadFileName, char *convertingFileName, size_t keySize, unsigned int blockSize);
void WriteToBlockFile(Block block, size_t readedSize, FILE *block_fp, unsigned char *digestMessage);

int CheckBlock( Block block, size_t readedBlockSize );

void ConvertFileToBlockDone(char *convertingFileName, char *_convertingFileName, size_t fileNameSize);

void AESEncryptFile(char *filePath);
void AESDecryptFile(char *filePath);


#endif // _H_ConvertFile_
