#include "ConvertFile.h"
#include "SHAHash.h"

void FileToBlock(char *uploadFileName){
	char inputBuffer[BLOCKSIZE];
	unsigned char digestMessage[SHA256SIZE];
	char writeBlock[SHA256SIZE + BLOCKSIZE];
	FILE *upload_fp = fopen(uploadFileName, "rb");
	FILE *block_fp = fopen("BlockFolder/sample_image.ablock","wb");
	size_t _readedSize;


	for(;;){
		// Reach the end of the file
		puts("before");
		if ( ( _readedSize = fread(inputBuffer, 1, BLOCKSIZE, upload_fp )) < BLOCKSIZE ){
			puts("in");
			sha256Hash(inputBuffer, digestMessage);
			sprintf(writeBlock,"%s%s",digestMessage,inputBuffer);
			fwrite(writeBlock, 1, SHA256SIZE + _readedSize, block_fp);
			break;
		}
		puts("after");

		// get digest message
		sha256Hash(inputBuffer, digestMessage);

		puts("+++++++++++++");
		sprintf(writeBlock,"%s%s",digestMessage,inputBuffer);
		puts("-------------");

		fwrite(writeBlock, 1, SHA256SIZE + BLOCKSIZE, block_fp);
	}

	fclose(upload_fp);
	fclose(block_fp);
};


void BlockToFile(char *blockFileName){
	FILE *block_fp = fopen("BlockFolder/sample_image.ablock","rb");
	FILE *download_fp = fopen("DownloadFolder/sample_image.png","wb");
	size_t _readedSize;
	char inputBuffer[BLOCKSIZE + SHA256SIZE];
	char readedBlock[BLOCKSIZE];
	unsigned char readedDigestMessage[SHA256SIZE];

	size_t _readedDigestMessageSize, _readedBLOCKSIZE;

	for(;;){
		_readedDigestMessageSize = fread(readedDigestMessage, 1, SHA256SIZE, block_fp);

		if (( _readedBLOCKSIZE = fread(readedBlock, 1, BLOCKSIZE, block_fp) < BLOCKSIZE )){
			fwrite(readedBlock, 1, _readedBLOCKSIZE, download_fp);
			break;
		}

		fwrite(readedBlock, 1, BLOCKSIZE, download_fp);
	}


	/*
	for(;;){
		if ( ( _readedSize = fread(inputBuffer, 1, BLOCKSIZE + SHA256SIZE, block_fp)) < (BLOCKSIZE + SHA256SIZE) ){
			sscanf(inputBuffer,"%s%s",readedDigestMessage,readedBlock);
			fwrite(readedBlock, 1, _readedSize, download_fp);
			break;
		}

		sscanf(inputBuffer,"%s%s",readedDigestMessage,readedBlock);
		fwrite(readedBlock, 1, BLOCKSIZE, download_fp);
	}
	*/

	fclose(block_fp);
	fclose(download_fp);
};
