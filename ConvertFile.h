#ifndef _H_ConvertFile_
#define _H_ConvertFile_

#include <stdio.h>
#include <string.h>

#define SHA256SIZE 32
#define BLOCKSIZE 64512

typedef struct BLOCK{
	char bodyContext[64512];
	unsigned char digestMessage[64];
}Block;


/*
typedef struct BLOCKKEY{
	unsigned char blockDigestMessage[64];
	unsigned int blockSize;
	// time createdAt;
	char fileType[10];
	// time lastAccess;
	// bitMap
}BlockKey;
*/

void FileToBlock(char *uploadFileName);
void BlockToFile(char *blockFileName);

#endif // _H_ConvertFile_
