#ifndef _H_SHAHash_
#define _H_SHAHash_

#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>

#define DIGEST_NAME_SHA256 "sha256"
#define DIGEST_NAME_SHA512 "sha512"

int sha512Hash(char *plainContext, unsigned char *digestMessage, unsigned int plainContextSize);
//unsigned char sha512Hash(char *plainContext);

#endif // _H_SHAHash_
