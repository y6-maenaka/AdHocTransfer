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

typedef struct BLOCK{
	char bodyContext[64512];
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


void FileToBlock(char *_uploadFileName);
void BlockToFile(char *blockFileName);
void CreateKeyFile(unsigned char *allDigestMessage, BlockKey key, char *_uploadFileName, char *_convertingFileName, size_t keySize, unsigned int blockSize);
void GetConvertingFileName(uuid_string_t uuidFileNameString);
void WriteToBlockFile(Block block, size_t _readedSize, FILE *block_fp, unsigned char *_digestMessage);

int CheckBlock(unsigned char *readedDigestMessage, char *readedBlock, unsigned int readedDigestMessageSize);

#endif // _H_ConvertFile_
