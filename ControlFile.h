#ifndef _H_ControlFile_
#define _H_ControlFile_

#include <stdio.h>
#include <uuid/uuid.h>

#define UUIDSIZE 16


unsigned int GetFileSize(FILE *fp);
void GenerateUUID(uuid_t uuid);
void GenerateUUIDString(uuid_string_t uuidFileNameString);


#endif // _H_ControlFile_
