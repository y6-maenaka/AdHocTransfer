#include "HashCipher.h"

// EVP_MAX_MD_SIZE = 64

int sha512Hash(char *plainContext, unsigned char *digestMessage, unsigned int plainContextSize){
	EVP_MD_CTX *mdctx; // メッセージダイジェストの状態を管理する構造体
	const EVP_MD *md; // メッセージダイジェスト関数を指定する構造体
	unsigned int mdLength;

	OpenSSL_add_all_digests(); // Setting internal table (internal structure)

	md = EVP_get_digestbyname(DIGEST_NAME_SHA512); // use SHA256 function(algorithm)

	if ( !md ){ // if DIGEST_NAME is incorrect
		fprintf(stderr,"Digest name %s is incorrect",DIGEST_NAME_SHA512);
		exit(1);
	}

	mdctx = EVP_MD_CTX_create(); // create digest context and initialize

	//EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestInit(mdctx,md);
	EVP_DigestUpdate(mdctx, plainContext, plainContextSize);
	EVP_DigestFinal_ex(mdctx, digestMessage, &mdLength); // mdLength is size of bytes

	EVP_MD_CTX_destroy(mdctx);
	EVP_cleanup();

	return mdLength;
}

unsigned char *AESEncrypt(const char *key, const char *encryptData, const size_t encryptDataLength, const unsigned char *iv, unsigned char *dest, const size_t destLength){

	EVP_CIPHER_CTX *encryptCTX; // 暗号化の内部状態を保持する
	int c_len = destLength;
	int f_len = 0;

	memset(dest, 0x00, destLength); // 暗号化データ(出力)をゼロクリアする

	encryptCTX = EVP_CIPHER_CTX_new(); // initialize

	EVP_EncryptInit_ex(encryptCTX, EVP_aes_128_cbc(), NULL, (unsigned char*)key, iv); // initialize  or EVP_EncryptInit()

	/*
	 	※ 初期化以外のEVP関数を呼び出す前にcontextは初期化しておく必要がある
	 	first argument : crypt context
		second argument : crypt function
		third argument : crypt engine
		forth argument : key (16bytes)
		fifth argument : vector (16bytes)
	 */

	EVP_EncryptUpdate(encryptCTX, dest, &c_len, (unsigned char *)encryptData, encryptDataLength);

	// 0 < data size < encryptDataLength + cipher_block_size - 1
	

	/*
	 	dest : output
	  data : input

	 */
	
	EVP_EncryptFinal_ex(encryptCTX, (unsigned char *)(dest + c_len), &f_len);
	EVP_CIPHER_CTX_cleanup(encryptCTX);


	return dest;
}

char *AESDecrypt(const char *key, const unsigned char *decryptData, const size_t decryptDataLength, const unsigned char *iv, char *dest, const size_t destLength ){

	EVP_CIPHER_CTX *decryptCTX;
	int p_len = decryptDataLength;
	int f_len = 0;

	memset(dest, 0x00, destLength);

	EVP_CIPHER_CTX_init(decryptCTX);
	EVP_DecryptInit_ex(decryptCTX, EVP_aes_128_cbc(), NULL, (unsigned char *)key, iv);

	EVP_DecryptUpdate(decryptCTX, (unsigned char *)dest, &p_len, decryptData, decryptDataLength);

	EVP_DecryptFinal_ex(decryptCTX, (unsigned char *)(dest + p_len), &f_len);
	EVP_CIPHER_CTX_cleanup(decryptCTX);

	return dest;
}
