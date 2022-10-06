#include "../header/ControlFile.h"
#include "../header/HashCipher.h"

static char AESKey[AES_KEY_SIZE];

long int GetFileSize(FILE *fp){

	unsigned int fileSize = -1;

	fseek(fp,0,SEEK_END);
	fileSize = ftell(fp);
	fseek(fp,0,SEEK_SET);

	if ( fileSize <= 0 ){
		fprintf(stderr, "can not get file size");
		exit(1);
	}

	return fileSize;
}

void GenerateUUID(uuid_t uuid){
	uuid_generate(uuid);
}


void GenerateUUIDString(char *uuidFileNameString){

	uuid_t uuidFileName;
	uuid_generate(uuidFileName);

	uuid_unparse_upper(uuidFileName, uuidFileNameString);

}

void LoadConfig(){

	FILE *config_fp;
	//extern char AESKey[AES_KEY_SIZE];

	srand((unsigned int)time(NULL));

	if ( (config_fp = fopen(".config/.AdHocTransfer.aconf", "rb")) ){
		fread( AESKey, AES_KEY_SIZE, 1, config_fp );
	}else{

		config_fp = fopen(".config/.AdHocTransfer.aconf","wb");

		for(int i=0; i<sizeof(AESKey); i++)
			AESKey[i] = rand() % 126 + 33;

			fwrite( AESKey, AES_KEY_SIZE, 1, config_fp );
	}

	fclose(config_fp);

}


char *GetAESKey(){
	return AESKey;
}
