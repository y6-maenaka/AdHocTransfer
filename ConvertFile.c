#include "ConvertFile.h"
#include "HashCipher.h"
#include "ControlFile.h"


void FileToBlock(char *_uploadFileName){

	// digestMessageの後にbodyMessageが格納される
	char inputBuffer[BLOCKSIZE];
	unsigned char _digestMessage[EVP_MAX_MD_SIZE]={0};
	// char writeBlock[EVP_MAX_MD_SIZE + BLOCKSIZE];

	char uploadFileName[255];
	sprintf(uploadFileName,"UploadFolder/%s",_uploadFileName);

	FILE *upload_fp = fopen(uploadFileName, "rb");
	size_t _readedSize;
	Block block={0};
	BlockKey key;

	unsigned int fileSize = GetFileSize(upload_fp);

	key.blockSize = fileSize;

	unsigned int allDigestMessageSize = (int)ceil( (double)fileSize / BLOCKSIZE );
	unsigned char *_allDigestMessage =  malloc( allDigestMessageSize * EVP_MAX_MD_SIZE );

	unsigned char allDigestMessage[EVP_MAX_MD_SIZE];

	/* Create ConvertingFile */

	uuid_string_t _convertingFileName;
	GetConvertingFileName(_convertingFileName);
	// folder % + . + uuid + .ablock [ 12 + 1 + 1 + 37 + 7 ]bytes
	char convertingFileName[BLOCKFOLDERNAMESIZE + 1+ 1 + sizeof(_convertingFileName) + BLOCKEXTENSIONSIZE ];
	sprintf(convertingFileName,"BlockFolder/.%%%s.ablock", _convertingFileName);

	FILE *block_fp = fopen(convertingFileName,"wb");

	int count = 0;

	for(;;){

		// Reach the end of the file
		if ( ( _readedSize = fread(block.bodyContext, 1, BLOCKSIZE, upload_fp )) < BLOCKSIZE ){

			WriteToBlockFile(block, _readedSize, block_fp, _digestMessage);
			memcpy(_allDigestMessage + (count * EVP_MAX_MD_SIZE) , _digestMessage , EVP_MAX_MD_SIZE);
			break;
		}

		WriteToBlockFile(block, _readedSize, block_fp, _digestMessage);

		/*
		const char key[16] = "abcdefghijklmnop";
		const unsigned char iv[16] = "abcdefghijklmnop";
		unsigned char encodeData[sizeof(block.bodyContext)] = {'\0'};

		AESEncrypt(key, block.bodyContext, sizeof(block.bodyContext), iv, encodeData, sizeof(encodeData));
		
	*/


		memcpy(_allDigestMessage + (count * EVP_MAX_MD_SIZE), _digestMessage, EVP_MAX_MD_SIZE);

		count += 1;
	}

	fclose(upload_fp);
	fclose(block_fp);


	// get allDigestMessage
	sha512Hash( (char *)_allDigestMessage, allDigestMessage , (allDigestMessageSize * EVP_MAX_MD_SIZE) );

	CreateKeyFile(allDigestMessage, key, _uploadFileName, _convertingFileName, sizeof(key), fileSize);

	// rename converting file to block file
	char BlockFileName[ BLOCKFOLDERNAMESIZE + 1 + sizeof(_convertingFileName) + BLOCKEXTENSIONSIZE ];
	sprintf(BlockFileName,"BlockFolder/%%%s.ablock", _convertingFileName);
	rename(convertingFileName, BlockFileName);

	free(_allDigestMessage);
};


void BlockToFile(char *blockFileName){
	FILE *block_fp = fopen("BlockFolder/sample_image.ablock","rb");
	FILE *download_fp = fopen("DownloadFolder/sample_image.png","wb");
	size_t _readedSize;
	char readedBlock[BLOCKSIZE]={0};
	unsigned char readedDigestMessage[EVP_MAX_MD_SIZE]={0};

	printf("%p\n",block_fp);	
	printf("%p",download_fp);


	size_t _readedDigestMessageSize, _readedBlockSize;

	int count = 0;
	for(;;){

		//printf("Converting Block To Original File ( %d )\n", count+1);
	
		_readedDigestMessageSize = fread(readedDigestMessage, 1, EVP_MAX_MD_SIZE, block_fp);

		if (( _readedBlockSize = fread(readedBlock, 1, BLOCKSIZE, block_fp) < BLOCKSIZE )){
			fwrite(readedBlock, 1, _readedBlockSize, download_fp);
			break;
		}
	
		//int tmp = CheckBlock(readedDigestMessage, readedBlock, _readedDigestMessageSize);
		//printf("%d\n",tmp);

		fwrite(readedBlock, 1, BLOCKSIZE, download_fp);
		count += 1;
	}

	fclose(block_fp);
	fclose(download_fp);
};


void CreateKeyFile(unsigned char *allDigestMessage, BlockKey key,char *_uploadFileName,char *_convertingFileName, size_t keySize, unsigned int blockSize){

	char keyFileName[255];

	sprintf(keyFileName, "KeyFolder/$%s.akey", _uploadFileName);

	FILE *key_fp = fopen(keyFileName,"wb");

	memcpy(key.blockDigestMessage, allDigestMessage, 64);
	memcpy(key.fileName, _uploadFileName, strlen(_uploadFileName));
	memcpy(key.blockName, _convertingFileName, 37);

	time(&key.createdAt);
	time(&key.lastAccess);

	fwrite( &key, keySize, 1, key_fp);

	fclose(key_fp);
};


void GetConvertingFileName(uuid_string_t uuidFileNameString){

	uuid_t uuidFileName;
	uuid_generate(uuidFileName);
	uuid_unparse_upper(uuidFileName, uuidFileNameString);

}


void WriteToBlockFile(Block block, size_t _readedSize, FILE *block_fp, unsigned char *_digestMessage){

	sha512Hash(block.bodyContext, block.digestMessage, _readedSize);
	memcpy(_digestMessage , block.digestMessage, EVP_MAX_MD_SIZE);

	fwrite(block.digestMessage, 1, EVP_MAX_MD_SIZE, block_fp);
	fwrite(block.bodyContext, 1, _readedSize, block_fp);

}

int CheckBlock(unsigned char *readedDigestMessage, char *readedBlock, unsigned int readedDigestMessageSize){
	
	unsigned char integrityDigestMessage[EVP_MAX_MD_SIZE];

	sha512Hash(readedBlock, integrityDigestMessage, readedDigestMessageSize);
	
	if( memcmp(readedDigestMessage, integrityDigestMessage, EVP_MAX_MD_SIZE ) == 0)
		return 0;
	else 
		return -1;
}
