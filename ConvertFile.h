#ifndef _H_ConvertFile_
#define _H_ConvertFile_

#include <stdio.h>
#include <string.h>

#define SHA256Length 256
#define SHA256SIZE 32
#define BLOCKSIZE 64000

void FileToBlock(char *uploadFileName);
void BlockToFile(char *blockFileName);

#endif // _H_ConvertFile_
