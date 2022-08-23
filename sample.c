#include <stdio.h>
#include <stdlib.h>

// https://www.phactory.jp/blog/c_open_bigfile/
// http://c-lang.sevendays-study.com/ex-day6.html

int main(){	
	FILE *fp;
	char filename[] = "UploadFolder/sample_video.mp4";

	FILE *fp_output;
	char output_filename[] = "BlockFolder/sample_output_video.mp4";
	char buffer[64000];
	size_t tmp;

	fp = fopen(filename,"rb"); // バイナリモードで読み込み
	
	fp_output = fopen(output_filename, "wb");
	
	while(1){

		if( (tmp = fread(buffer,1,64000,fp)) < 64000)
			break;
		fwrite(buffer,1,64000,fp_output);

	}
	fwrite(buffer,1,tmp,fp_output);

	fclose(fp);
	fclose(fp_output);

	// fopen() 別の書き方 fopen_s(&fp, "filename","mode");
}


/*
	size_t fread(void *buf, size_t size, size_t num, FILE *fp);
	第一引数 : 読み込んだデータを格納するメモリへのポインタ
	第二引数 : 読み込み要素一つあたりのバイトサイズ
	第三引数 : 読み込み要素の個数
	第四引数 : 読み込み対象へのファイルハンドル　ファイル位置

	戻り値 : 読み込みできた要素の個数
	
*/
