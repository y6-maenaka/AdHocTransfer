#include "SHAHash.h"
#include "ConvertFile.h"
#include <stdio.h>


int main(int argc, char *argv[]){
	/*
	char message[] = "Hello";
	unsigned char mdValue[EVP_MAX_MD_SIZE];
	int tmp = sha256Hash(message,mdValue);

	for(int i=0;i<tmp;i++)
		printf("%02x",mdValue[i]);
	*/

	char fileName[] = "UploadFolder/sample_image.png";
	FileToBlock(fileName);

	//char fileName[] = "BlockFolder/sample_image.ablock";
	//BlockToFile(fileName);
	
}
