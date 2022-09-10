#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#define RSA_KEY_LENGTH 4096

static const char rnd_seed[] = "random seed";

int main(){

	/*
	RSA *prikey, *pubkey;
	prikey = RSA_new();
	char *p,*q,*n,*e,*d;

	BIGNUM *bignum;
	//bignum = BN_new();
  int ret = BN_set_word(bignum, RSA_F4);

	ret =	RSA_generate_key_ex(prikey, KEYBIT_LEN, bignum, NULL);
	BN_free(bignum);
	RSA_free(prikey);

	EVP_RSA_gen(KEYBIT_LEN);
	*/


	// BIGNUMライブラリはデータの構造の格納に動的メモリ確保が行われているため、扱える数値に上限はない。
	// このライブラリは多倍長上限を保持するために用いられる。　この構造体を直接参照したり、変更してはいけない

	/*
	 typedef struct bignum_st{
			int top;
			BN_ULONG *d;
			int max;
			int neg;
	 } BIGNUM;
	 */

	// generate privatr key & public key


	// RSA_generate_key : RSA鍵ペアを作成する


	//prikey = RSA_generate_key(KEYBIT_LEN, RSA_F4, NULL, NULL);  // 非推奨 OpenSSL 0.9.8〜

	//prikey = EVP_RSA_gen(KEYBIT_LEN);


	//int RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);
	//RSA_generate_key_ex(prikey, KEYBIT_LEN, BIGNUM *e, BN_GENCB *cb);


	/*
	 RSA_generate_key(int bits, unsigned long e, void (*callback)(int, int, void *), void *cb_arg);
				RSA_F4 :  暗号化指数(standard,default -> e = 65337)
	 */

	/*
	if (prikey == NULL){
		printf("error");
		return 1;
	}
	priKeyFile = fopen("RSAPriKey.pem","w");
	if(priKeyFile == NULL){
		perror("failed to fopen");
		return 1;
	}
	*/


	/*
	BN_bn2bin -> フォーマット変換

	char *BN_bn2hex(const BIGNUM *a);

	BN_bn2hex()とBN_bn2dec()は、それぞれaの16進数と10進数のエンコードを含む印刷可能な
	文字列を返す。　負の数値の場合、文字列の前に[-]がつく。
	文字列は後でOPENSSL_free()を使用して解放する必要がある。
	 */

	/*
	p = BN_bn2hex(prikey->p);
	q = BN_bn2hex(prikey->q);
	n = BN_bn2hex(prikey->n);
	e = BN_bn2hex(prikey->e);
	d = BN_bn2hex(prikey->d);
	printf("p = 0x%s\n", p);
	printf("q = 0x%s\n", q);
	printf("n = 0x%s\n", n);
	printf("e = 0x%s\n", e);
	printf("d = 0x%s\n", d);
	*/

	// 公開鍵暗号アルゴリズムを実現するライブラリで用いられる基本オブジェクト(構造体)は以下

	/*
	typedef struct evp_pkey_st EVP_PKEY; // 公開鍵暗号用の鍵構造体
								// アルゴリズム固有の鍵情報（公開・秘密鍵）パラメータを含む
	typedef struct evp_pkey_xtx_st EVP_PKEY_CTX // 公開鍵暗号コンテキストの構造体
								// EVP_PKEY、鍵生成情報、アルゴリズム固有のパラメータなどを含む
	*/


	/*
	EVP_PKEY *prikey = NULL;
	prikey = EVP_PKEY_new();

	unsigned long rsa_n = 0xbc747fc5;
	unsigned long rsa_e = 0x10001;
	unsigned long rsa_d = 0x7b133399;

	OSSL_PARAM params[] = {
	OSSL_PARAM_ulong("n", &rsa_n),
	OSSL_PARAM_ulong("e", &rsa_e),
	OSSL_PARAM_ulong("d", &rsa_d),
	OSSL_PARAM_END
	};


	EVP_PKEY_CTX *pkey_ctx;
	EVP_PKEY_CTX_new(prikey, NULL);
	EVP_PKEY_keygen_init(pkey_ctx);

	EVP_PKEY_fromdata_init(pkey_ctx);
	EVP_PKEY_fromdata(pkey_ctx, &prikey, EVP_PKEY_KEYPAIR, params);

	//EVP_PKEY_CTX_new(prikey, NULL );

	puts("+++++++++");
	// error happen ↓
	EVP_PKEY_keygen(pkey_ctx, &prikey);

	int tmp = EVP_PKEY_size(prikey);
	printf("%d",tmp);

	EVP_PKEY_free(prikey);
	EVP_PKEY_CTX_free(pkey_ctx);
	*/

	/*
	EVP_PKEY_new(); // 空の鍵構造体を生成
	EVP_PKEY_free();  // 鍵構造体keyを解放する




	EVP_PKEY_CTX_new();  // ctxを生成
	or
	EVP_PKEY_CTX_new_id(); // ctxを生成

	EVP_PKEY_CTX_dup(); // 複製
	EVP_PKEY_CTX_free(); // ctxを解放

	// =================================================

	EVP_PKEY_keygen_init(); // ctxを鍵生成処理のために初期化する
	EVP_PKEY_keygen(); // 鍵生成処理　生成されたカギはppkeyに書き込まれる

	EVP_PKEY_paramgen(); // パラメータ生成処理

	EVP_PKEY_set1_XXX(); // pkeyによって参照される鍵をkeyに設定する

	EVP_PKEY_type(type); // typeに対応する鍵タイプを帰る

	*/


	// =========[ 初期パラメータを指定しない場合 ]=============
	// キーの書き出し

	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL); // ctxの生成
	//EVP_PKEY *pkey = EVP_RSA_gen(KEYBIT_LEN);
	EVP_PKEY *pkey = NULL;

	EVP_PKEY_keygen_init(pctx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, 4096);
	EVP_PKEY_keygen(pctx, &pkey); // pkeyはpub,pri,etc(params)を保持している
	
	EVP_PKEY_free(pkey);

	// =================================
	
	// BIO = openssl->I/Oを抽象化する・緩衝の役割
	
	//FILE *pubric_pem_fp = fopen("pubKey.pem","w");
	//PEM_write_PUBKEY(pubric_pem_fp, pkey);
	//fclose(pubric_pem_fp);
	

	//FILE *private_pem_fp = fopen("priKey.pem","w");	
	//PEM_write_PrivateKey(private_pem_fp, pkey,NULL,NULL,0,NULL,NULL);
	//fclose(private_pem_fp);
	

	// =================================
	
	
	unsigned char cryptMess[53] = "hello world this is sample message to crypt with RSA";
	size_t out_len;
	unsigned char *out;


	FILE *pem_fp = fopen("pubKey.pem","r");
	EVP_PKEY *epubKey = PEM_read_PUBKEY(pem_fp, NULL, NULL, NULL);
	//EVP_PKEY *epuKey = PEM_read_PrivateKey(pem_fp, NULL , NULL, NULL);
	
	EVP_PKEY_CTX *_ctx = EVP_PKEY_CTX_new(epubKey, NULL);

	
	EVP_PKEY_encrypt_init(_ctx);
	EVP_PKEY_CTX_set_rsa_padding(_ctx, RSA_PKCS1_PADDING);
	EVP_PKEY_encrypt(_ctx, out, &out_len, cryptMess, sizeof(cryptMess));


	EVP_PKEY_encrypt(_ctx, NULL, &out_len, cryptMess, sizeof(cryptMess));

	out = OPENSSL_malloc(out_len);
	EVP_PKEY_encrypt(_ctx, out, &out_len, cryptMess, sizeof(cryptMess));
	
	puts("====== encrypt below ========== ");
	printf("%s\n",out);

	EVP_PKEY_free(epubKey);
	EVP_PKEY_CTX_free(_ctx);

	fclose(pem_fp);



	size_t tmp;
	unsigned char *_out;


	FILE *_pem_fp = fopen("priKey.pem","r");
	EVP_PKEY *_epubKey = PEM_read_PrivateKey(_pem_fp, NULL, NULL, NULL);

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(_epubKey, NULL);

	EVP_PKEY_decrypt_init(ctx);
	EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);

	EVP_PKEY_decrypt(ctx, NULL, &tmp, out, out_len);

	_out = OPENSSL_malloc(tmp);

	EVP_PKEY_decrypt(ctx, _out, &tmp, out, out_len);

	puts("============ decrypt below ================");
	printf("%s",_out);


	fclose(_pem_fp);

	FILE *sample_fp = fopen("samplePubKey.pem","w");
	PEM_write_PUBKEY(sample_fp, _epubKey);
	fclose(sample_fp);



	/*

	unsigned char cryptMess[15] = "sample message";
	size_t out_len;
	unsigned char *out;

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(XXXX, NULL);

	EVP_PKEY_encrypt_init(ctx);

	EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);

	EVP_PKEY_encrypt(ctx,NULL, &out_len,cryptMess,sizeof(cryptMess) );

	out = OPENSSL_malloc(out_len);

	EVP_PKEY_encrypt(ctx, out, &out_len, cryptMess, sizeof(cryptMess));



	unsigned char aa[out_len];
	memcpy(aa, out, out_len);


	size_t tmp;
	unsigned char *_out;

	EVP_PKEY_CTX *_ctx = EVP_PKEY_CTX_new(pkey, NULL);
	EVP_PKEY_decrypt_init(_ctx);
	EVP_PKEY_CTX_set_rsa_padding(_ctx, RSA_PKCS1_PADDING);
	EVP_PKEY_decrypt(_ctx, NULL, &tmp, aa, sizeof(aa));
	_out = OPENSSL_malloc(tmp);

	EVP_PKEY_decrypt(_ctx, _out, &tmp, aa, sizeof(aa));

	printf("%s",_out);



	*/


	//int tmp = EVP_PKEY_size(pkey);
	//printf("%d",tmp);

	// or
	/*
	EVP_PKEY *pkey = NULL;
	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);
	EVP_PKEY_keygen_init(pctx);
	EVP_PKEY_generate(pctx, &pkey);
	EVP_PKEY_CTX_free(pctx);
	*/

	/*
	unsigned int primes = 3;
	unsigned int bits = 4096;
	OSSL_PARAM params[3];
	EVP_PKEY *pkey = NULL;
	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);

	EVP_PKEY_keygen_init(pctx);

	params[0] = OSSL_PARAM_construct_unit("bits", &bits);
	params[1] = OSSL_PARAM_construct_unit("primes", &primes);
	params[2] = OSSL_OARAM_construct_end();
	EVP_PKEY_CTX_set_params(pctx, params);

	EVP_PKEY_generate(pctx, &pkey);
	EVP_PKEY_print_private(bio_out, pkey, 0, NULL);
	EVP_PKEY_CTX_free(pctx);
	*/

}
