#include "HashCipher.h"
#include "ConvertFile.h"
#include "ControlFile.h"
#include <stdio.h>

#define KEYSIZE 16


int main(int argc, char *argv[]){
	//char fileName[] = "sample_video.mp4";
	//FileToBlock(fileName);

	//char fileName[] = "sample_image.ablok";
	//BlockToFile(fileName);
	
	//AESEncryptFile("BlockFolder/%42FDC580-EA82-4867-8B76-BFF1506085BD.ablock");
	//AESDecryptFile("BlockFolder/%42FDC580-EA82-4867-8B76-BFF1506085BD.ablock");

	
	/*
	char key[16] = "abcdefghijklmnop";
	char cryptData[] = "hello, OpenSSL! 123456789012345";
	unsigned char iv[16] = "abcdefghijklmnop";
	char decodeData[64] = {'\0'};

	unsigned char *sample;
	char *_sample;

	//datasize = datasize + ( blocksize - (datasize % block_size))
	
	size_t dataSize = sizeof(cryptData) + ( AES_BLOCK_SIZE - (sizeof(cryptData) % AES_BLOCK_SIZE));


		
	sample = malloc(GetAESDecryptedDataSize(sizeof(cryptData)));
	_sample = malloc(dataSize);

	int tmp = AESEncrypt(key , cryptData, sizeof(cryptData),iv, sample);
	int _tmp = AESDecrypt(key, sample, dataSize, iv, _sample);


	for(int i=0; i<dataSize; i++) printf("%c",sample[i]);

	//printf("%s",_sample);

	free(sample);
	free(_sample);
	*/	

	/*
	for(int i=0; i < sizeof(_u); i++)
		printf("%c",_u[i]);
	*/

	/*	
	uuid_t uuid;
	GenerateUUID(uuid); // 128bit 32words

	
	FILE *config_fp;
	char key[sizeof(uuid)] = {0};

	memcpy(key, (char *)uuid, sizeof(uuid));

	if( (config_fp = fopen("config/.AdHocTransfer.aconf","rb")) ){
		fread(key, sizeof(uuid), 1, config_fp);
		printf("%s\n",key);

	}else{
		config_fp = fopen("config/.AdHocTransfer.aconf","wb");
		fwrite(key, sizeof(uuid), 1, config_fp );
	}

	fclose(config_fp);
	*/


	//LoadConfig();
	

	/* ======= RSA =============================

	EVP_PKEY *pkey = NULL;	
	//GenerateRSAPrivateKey(pkey);
	
	unsigned char sampleMess[23] = "This is Sample Message";
	unsigned char *tmp;
	unsigned char *_tmp;

	size_t outLength, _outLength;

	
	pkey = ReadRSAPublicKey();
	tmp = RSAEncrypt(pkey, sampleMess, sizeof(sampleMess), &outLength);



	printf("%s\n",tmp);

	puts("+++++++++++++++==");
	EVP_PKEY *_pkey = NULL;
	_pkey = ReadRSAPrivateKey();
	_tmp = RSADecrypt(_pkey, tmp, outLength, &_outLength );

	printf("%s",_tmp);

	OPENSSL_free(tmp);

	============================================================ */

}

