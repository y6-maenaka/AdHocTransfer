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

int AESEncrypt(const char *key, const char *encryptData, const size_t encryptDataLength,const unsigned char *iv, unsigned char *dest){

	EVP_CIPHER_CTX *encryptCTX = NULL; // 暗号化の内部状態を保持する
	int c_len;
	int f_len = 0;

	if ( !(encryptCTX = EVP_CIPHER_CTX_new()) ) CryptoError("EVP_CIPHER_CTX_new() failure");

	//if ( ! EVP_CIPHER_CTX_init(encryptCTX) ) CryptoError("EVP_CIPHER_CTX_init() failure");
	

 // initialize  or EVP_EncryptInit()
	if ( EVP_EncryptInit_ex(encryptCTX, EVP_aes_128_cbc(), NULL, (unsigned char *)key, NULL) <= 0 )
		CryptoError("EVP_EncryptInit_ex() failure");

	/*
	 	※ 初期化以外のEVP関数を呼び出す前にcontextは初期化しておく必要がある
	 	first argument : crypt context
		second argument : crypt function
		third argument : crypt engine
		forth argument : key (16bytes)
		fifth argument : vector (16bytes)
	 */

	if ( EVP_EncryptUpdate(encryptCTX, dest, &c_len, (unsigned char *)encryptData, encryptDataLength) <= 0 ) 

	// 0 < data size < encryptDataLength + cipher_block_size - 1	
	
	EVP_EncryptFinal_ex(encryptCTX, dest + c_len, &f_len);

	EVP_CIPHER_CTX_cleanup(encryptCTX);
	//EVP_CIPHER_CTX_free(encryptCTX);

	return f_len;
}

int AESDecrypt(const char *key, const unsigned char *decryptData, const size_t decryptDataLength, const unsigned char *iv, char *dest){


	EVP_CIPHER_CTX *decryptCTX = NULL;

	if ( !(decryptCTX = EVP_CIPHER_CTX_new())) 
		CryptoError("EVP_CIPHER_CTX_new() failure");

	int c_len;
	int f_len = 0;

	if ( EVP_DecryptInit_ex(decryptCTX, EVP_aes_128_cbc(), NULL, (unsigned char *)key, NULL) <= 0)
		CryptoError("EVP_DecryptInit_ex() failure");

	if ( EVP_DecryptUpdate(decryptCTX, (unsigned char *)dest, &c_len, decryptData, decryptDataLength) <= 0 )
		CryptoError("EVP_DecryptUpdate() failure");
	
	printf("%d",c_len);

	EVP_DecryptFinal_ex(decryptCTX, (unsigned char *)(dest + c_len), &f_len);

	EVP_CIPHER_CTX_cleanup(decryptCTX);
	//EVP_CIPHER_CTX_free(decryptCTX);

	return f_len;
}


unsigned int GetAESEncryptedDataSize(size_t fileSize){
	return fileSize + ( AES_BLOCK_SIZE - ( fileSize % AES_BLOCK_SIZE ));
};



void GenerateRSAPrivateKey(EVP_PKEY *pkey){
	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);

	if(	EVP_PKEY_keygen_init(pctx) <= 0 )
		CryptoError("EVP_PKEY_keygen_init() failure");

	if ( EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, RSA_KEY_SIZE) <= 0 )
		CryptoError("EVP_PKEY_CTX_set_rsa_keygen_bits() failure");

	if ( EVP_PKEY_keygen(pctx, &pkey) <= 0)
		CryptoError("EVP_PKEY_keygen() failure");

	SaveRSAPublicKey(pkey);
	SaveRSAPrivateKey(pkey);

	FreeEVPResource(pkey, pctx);
}


void SaveRSAPublicKey(EVP_PKEY *pkey){

	FILE *publicKeyPem_fp;
	publicKeyPem_fp = fopen(PUBLIC_KEY_PEM_PATH,"w");

	if (publicKeyPem_fp == NULL) 	fprintf(stderr,"can not open public key pem file");

	if ( PEM_write_PUBKEY(publicKeyPem_fp, pkey) <= 0 )
		CryptoError("PEM_write_PUBKEY() failure");

	fclose(publicKeyPem_fp);

};


void SaveRSAPrivateKey(EVP_PKEY *pkey){

	FILE *privateKeyPem_fp;
	privateKeyPem_fp = fopen(PRIVATE_KEY_PEM_PATH,"w");
	
	if ( privateKeyPem_fp == NULL )  fprintf(stderr, "can not open private key pem file");

	if ( PEM_write_PrivateKey(privateKeyPem_fp, pkey, NULL, NULL, 0, NULL, NULL)  <=  0)
		CryptoError("PEM_write_PrivateKey() failure");

	fclose(privateKeyPem_fp);
};



EVP_PKEY *ReadRSAPublicKey(){

	FILE *publicKeyPem_fp;
	if ( ( publicKeyPem_fp = fopen(PUBLIC_KEY_PEM_PATH, "r")) == NULL )
		fprintf(stderr, "can not open public key pem file");

	EVP_PKEY *publicKey = PEM_read_PUBKEY(publicKeyPem_fp, NULL, NULL, NULL);
	if ( publicKey == NULL)
		CryptoError("PEM_read_PUBKEY() failure");

	fclose(publicKeyPem_fp);

	return publicKey;
};



EVP_PKEY *ReadRSAPrivateKey(){

	FILE *privateKeyPem_fp;
	privateKeyPem_fp = fopen(PRIVATE_KEY_PEM_PATH, "r");
	if ( privateKeyPem_fp == NULL)
		fprintf(stderr,"can not open private key pem file");

	EVP_PKEY *privateKey = PEM_read_PrivateKey(privateKeyPem_fp, NULL, NULL, NULL);
	if (privateKey == NULL)
		CryptoError("PEM_read_PrivateKey() failure");
	
	fclose(privateKeyPem_fp);

	return privateKey;
};



unsigned char *RSAEncrypt(EVP_PKEY *pkey, unsigned char *plainMess, size_t plainMessLength, size_t *outLength){

	unsigned char *encryptedMess;

	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new(pkey, NULL);
	
	if ( EVP_PKEY_encrypt_init(pctx) <= 0 )
		CryptoError("EVP_PKEY_encrypt_init() failure");

	if ( EVP_PKEY_CTX_set_rsa_padding(pctx, RSA_PKCS1_PADDING) <= 0 )
		CryptoError("EVP_PKEY_CYX_set_rsa_padding() failure");

	if ( EVP_PKEY_encrypt(pctx, NULL, outLength, plainMess, plainMessLength) <= 0 )
		CryptoError("EVP_PKEY_encrypt() failure");

	encryptedMess = OPENSSL_malloc(*outLength);
	if ( EVP_PKEY_encrypt(pctx, encryptedMess, outLength, plainMess, plainMessLength) <= 0)
		CryptoError("EVP_PKEY_encrypt() failure");

	FreeEVPResource(pkey, pctx);

	return encryptedMess;
};


unsigned char *RSADecrypt(EVP_PKEY *pkey, unsigned char *encryptedMess, size_t encryptedMessLength, size_t *outLength){
	unsigned char *decryptedMess;

	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new(pkey, NULL);

	if ( EVP_PKEY_decrypt_init(pctx) <= 0 )
		CryptoError("EVP_OKEY_decrypto_init() failure");

	if ( EVP_PKEY_CTX_set_rsa_padding(pctx, RSA_PKCS1_PADDING) <= 0 )
		CryptoError("EVP_PKEY_CTX_set_rsa_paddin() failure");

	if ( EVP_PKEY_decrypt(pctx, NULL, outLength, encryptedMess, encryptedMessLength) <= 0) 
		CryptoError("EVP_PKEY_decrypt() failure");

	decryptedMess = OPENSSL_malloc(*outLength);

	if ( EVP_PKEY_decrypt(pctx, decryptedMess, outLength, encryptedMess, encryptedMessLength) <= 0)
		CryptoError("EVP_PKEY_decrypt() failure");

	FreeEVPResource(pkey, pctx);

	return decryptedMess;
};



void FreeEVPResource(EVP_PKEY *pkey, EVP_PKEY_CTX *pctx){
	EVP_PKEY_free(pkey);
	EVP_PKEY_CTX_free(pctx);
};



void CryptoError(char *errorMessage){
	perror(errorMessage);
	exit(1);
};
