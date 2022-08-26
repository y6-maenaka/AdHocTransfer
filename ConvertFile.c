#include "ConvertFile.h"
#include "SHAHash.h"

void FileToBlock(char *uploadFileName){

	// digestMessageの後にbodyMessageが格納される
	char inputBuffer[BLOCKSIZE];
	unsigned char digestMessage[EVP_MAX_MD_SIZE]={0};
	// char writeBlock[EVP_MAX_MD_SIZE + BLOCKSIZE];
	FILE *upload_fp = fopen(uploadFileName, "rb");
	FILE *block_fp = fopen("BlockFolder/sample_video.ablock","wb");
	size_t _readedSize;
	Block block={0};

	for(;;){
		// Reach the end of the file
		if ( ( _readedSize = fread(block.bodyContext, 1, BLOCKSIZE, upload_fp )) < BLOCKSIZE ){
			sha512Hash(block.bodyContext, block.digestMessage, _readedSize);

			/*
			sprintf(writeBlock,"%s%s",digestMessage,block.bodyContext);
			fwrite(writeBlock, 1, EVP_MAX_MD_SIZE + _readedSize, block_fp);
			*/

			fwrite(block.digestMessage, 1, EVP_MAX_MD_SIZE, block_fp);
			fwrite(block.bodyContext, 1, _readedSize,  block_fp);
			break;
		}

		sha512Hash(block.bodyContext, block.digestMessage, _readedSize); // get digest message

		/*
		sprintf(writeBlock,,"%s%s",digestMessage, block.bodyContext);
		fwrite(writeBlock, 1, EVP_MAX_MD_SIZE + _readedSize, block_fp);
		*/
		fwrite(block.digestMessage, 1, EVP_MAX_MD_SIZE,  block_fp);
		fwrite(block.bodyContext, 1, _readedSize, block_fp);
	}

	fclose(upload_fp);
	fclose(block_fp);
};


void BlockToFile(char *blockFileName){
	FILE *block_fp = fopen("BlockFolder/sample_video.ablock","rb");
	FILE *download_fp = fopen("DownloadFolder/sample_video.mp4","wb");
	size_t _readedSize;
	//char inputBuffer[BLOCKSIZE + EVP_MAX_MD_SIZE];
	char readedBlock[BLOCKSIZE]={0};
	unsigned char readedDigestMessage[EVP_MAX_MD_SIZE]={0};

	size_t _readedDigestMessageSize, _readedBlockSize;

	for(;;){
		_readedDigestMessageSize = fread(readedDigestMessage, 1, EVP_MAX_MD_SIZE, block_fp);

		if (( _readedBlockSize = fread(readedBlock, 1, BLOCKSIZE, block_fp) < BLOCKSIZE )){
			fwrite(readedBlock, 1, _readedBlockSize, download_fp);
			break;
		}
		
		fwrite(readedBlock, 1, BLOCKSIZE, download_fp);
	}

	fclose(block_fp);
	fclose(download_fp);
};
