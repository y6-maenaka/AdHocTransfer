#include "HashCipher.h"
#include "ConvertFile.h"
#include "ControlFile.h"
#include <stdio.h>

#define KEYSIZE 37


int main(int argc, char *argv[]){
	//char fileName[] = "sample_image.png";
	//FileToBlock(fileName);

	//char fileName[] = "BlockFolder/sample_image.ablok";
	//BlockToFile(fileName);

	/*
	const char key[16] = "abcdefghijklmnop";
	const char cryptData[] = "hello, OpenSSL! 123456789012345\0";
	const unsigned char iv[16] = "abcdefghijklmnop";
	unsigned char encodeData[64] = {'\0'};
	char decodeData[64] = {'\0'};

	AESEncrypt(key , cryptData, sizeof(cryptData), iv, encodeData, sizeof(encodeData));
	AESDecrypt(key, encodeData, sizeof(encodeData), iv, decodeData, sizeof(decodeData));

	//printf("%s\n",encodeData);
	//printf("%s",decodeData);
	*/

	uuid_t _u;
	uuid_generate(_u);
	printf("%lu\n",sizeof(_u));

	for(int i=0; i < sizeof(_u); i++)
		printf("%c",_u[i]);

	uuid_t uuid;
	GenerateUUID(uuid);

	//for(int i=0; i< UUIDSIZE; i++)
		//printf("%x",uuid[i]);
	
	//FILE *config_fp = fopen("config/AdHocTransfer.aconf","w");
	char tmp[64];
	sprintf(tmp,"KEY:%s",uuid);
	//printf("%s",tmp);
	//fwrite(tmp,sizeof(tmp),1 ,config_fp);

	//printf("%s",key);

	//uuid_t uuid;
	//GenerateUUID(uuid);

	/*
	uuid_string_t uuid_string;
	GenerateUUIDString(uuid_string);
	printf("%s\n",uuid_string);
	printf("%lu",sizeof(uuid_string));
	*/
	

}

