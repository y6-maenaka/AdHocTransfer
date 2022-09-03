#include "ControlFile.h"

unsigned int GetFileSize(FILE *fp){

	unsigned int fileSize = -1;

	fseek(fp,0,SEEK_END);
	fileSize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	return fileSize;

}

void GenerateUUID(uuid_t uuid){
	uuid_generate(uuid);
}


void GenerateUUIDString(uuid_string_t uuidFileNameString){

	uuid_t uuidFileName;
	uuid_generate(uuidFileName);
	uuid_unparse_upper(uuidFileName, uuidFileNameString);

}
