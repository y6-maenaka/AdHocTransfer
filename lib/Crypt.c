#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>

#define LEN 64


unsigned char *Encrypt(const char *key, const char *data, const size_t datalen, const unsigned char *iv, unsigned char *dest, const size_t destlen){
	EVP_CIPHER_CTX *en;
	int i, f_len = 0;
	int c_len = destlen;

	memset(dest, 0x00, destlen);

	en = EVP_CIPHER_CTX_new();

	EVP_EncryptInit_ex(en, EVP_aes_128_cbc(), NULL, (unsigned char*)key, NULL);

	EVP_EncryptUpdate(en, dest, &c_len, (unsigned char *)data, datalen);
	EVP_EncryptFinal_ex(en, (unsigned char *)(dest + c_len), &f_len);
	EVP_CIPHER_CTX_cleanup(en);

	return dest;
}

char *Decrypt(const char *key, const unsigned char *data, const size_t datalen, const unsigned char *iv, char *dest, const size_t destlen ){
	EVP_CIPHER_CTX *de;
	int f_len = 0;
	int p_len = datalen;

	memset(dest, 0x00, destlen);

	EVP_CIPHER_CTX_init(de);
	EVP_DecryptInit_ex(de, EVP_aes_128_cbc(), NULL, (unsigned char*)key, NULL);

	printf("%s\n", data);
	printf("%zu", datalen);

	EVP_DecryptUpdate(de, (unsigned char *)dest, &p_len, data, datalen);

	EVP_DecryptFinal_ex(de, (unsigned char *)(dest + p_len), &f_len);

	EVP_CIPHER_CTX_cleanup(de);

	return dest;
}



int main(){
		const char key[16] = "abcdefghijklmnop"; // 暗号化に使う鍵 16バイト
		const char data[] = "hello, OpenSSL! 123456789012345"; // 暗号化するデータ
		const unsigned char iv[16] = "abcdefghijklmnop"; // 初期ベクトル 16バイト

		unsigned char encode[64] = {'\0'};
		char decode[64] = {'\0'};

		Encrypt(key, data, sizeof(data), iv, encode, sizeof(encode));
		Decrypt(key, encode, sizeof(encode), iv, decode, sizeof(decode));

		//printf("%s",encode);
		//puts("++++++++++++++++++");
		//printf("%s",decode);
}
