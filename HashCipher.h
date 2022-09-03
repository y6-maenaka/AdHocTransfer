#ifndef _H_HashCipher_
#define _H_HashCipher_

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <stdio.h>
#include <string.h>

#define DIGEST_NAME_SHA256 "sha256"
#define DIGEST_NAME_SHA512 "sha512"

int sha512Hash(char *plainContext, unsigned char *digestMessage, unsigned int plainContextSize);

unsigned char *AESEncrypt(const char *key, const char *encryptData, const size_t encryptDataLength, const unsigned char *iv, unsigned char *dest, const size_t destLength);

char *AESDecrypt(const char *key, const unsigned char *decryptData, const size_t decryptDataLength, const unsigned char *iv, char *dest, const size_t destLength);

// dest -> destination

#endif // _H_HashCipher_
