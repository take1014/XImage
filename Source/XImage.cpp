#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "XImage.h"
#include "_XImage.h"
#include "ErrorCode.h"
#include "XImageUtil.h"

// イメージファイル読取り
int XImage::ReadImage(char *FilePath){
	FILE *fop;		// ファイルオープン用のポインタ
	char buffer[MAX_BUFFERSIZE];
	int max_gray;
	// ファイルのオープン
	fop = fopen(FilePath,"rb");
	// ファイルのオープンに失敗した場合はエラー
	if(fop == NULL){
		return ERR_ReadImage_FOPEN;
	}

	// ファイルタイプの確認
	fgets(buffer,MAX_BUFFERSIZE,fop);

	// グレースケール（raw）形式以外の場合はエラー
	if(buffer[0] != 'P' || buffer[1] != '5'){
		return ERR_ReadImage_FILETYPE;
	}

	// x_size1,y_size1の代入
	x_size1 = 0;
	y_size1 = 0;
	// #があった場合は読み飛ばし、#がない場合は
	// 縦横幅を取得する
	while(x_size1 == 0 || y_size1 == 0){
		fgets(buffer,MAX_BUFFERSIZE,fop);
		if(buffer[0] != '#'){
			sscanf(buffer,"%d %d",&x_size1,&y_size1);
		}
	}
	
	// 入力イメージの動的確保を行う
	unsigned char **image =  MakeImageStruct(x_size1,y_size1);
	
	// メンバ変数に動的確保したメモリのアドレスを格納する
	image1 = image;

	// max_grayの代入
	max_gray = 0;
	while(max_gray == 0){
		fgets(buffer,MAX_BUFFERSIZE,fop);
		if(buffer[0] != '#'){
			sscanf(buffer,"%d",&max_gray);
		}
	}

	if(x_size1 > MAX_IMAGESIZE || y_size1 > MAX_IMAGESIZE){
		return ERR_ReadImage_IMAGESIZE;
	}
	if(max_gray != MAX_BRIGHTNESS){
		return ERR_ReadImage_BRIGHTNESS;
	}

	// 画像データを読み込んでメンバ変数に格納する
	for(int y = 0;y < y_size1; y++){
		for(int x = 0; x < x_size1; x ++){
		image[y][x] = (unsigned char)fgetc(fop);
		}
	}

	// イメージをメンバ変数に格納する
	image1 = image;
	fclose(fop);

	return OK;
}

// イメージファイル保存
int XImage::SaveImage(char *FileName){
	FILE *fop;	// ファイルオープン用のポインタ
	// 保存用のファイルポインタを開く
	fop = fopen(FileName,"wb");
	// ファイル識別子"P5"を先頭に出力する
	fputs("P5\n",fop);
	// #で始まるコメント行
	fputs("#Created by XImage\n",fop);
	// 画像の縦横幅を出力する
	fprintf(fop,"%d %d\n",x_size2,y_size2);
	// 最大階調値の出力
	fprintf(fop,"%d\n",MAX_BRIGHTNESS);

	// 画像データの出力
	for(int y = 0; y < y_size2; y++){
		for(int x = 0; x < x_size2;x++){
			fputc(image2[y][x],fop);
		}
	}
	fclose(fop);

	// 使用したイメージ領域の解放
	DeleteImageStruct(image1);
	DeleteImageStruct(image2);

	return OK;
}

// イメージ領域の動的確保
unsigned char** XImage:: MakeImageStruct(int xsize,int ysize){
	// メモリの動的確保
	unsigned char **image =(unsigned char**)malloc(sizeof(unsigned char*)*ysize);
	for(int i = 0; i < ysize; i++){
		image[i] = new unsigned char[xsize];
	}

	return image;
}

// イメージ領域の解放
void XImage::DeleteImageStruct(unsigned char **image){
	// イメージ領域解放
	free(image);
}

// イメージ反転処理
int XImage::InverseImage(){
	x_size2 = x_size1;
	y_size2 = y_size1;
	
	// 出力用イメージ画像格納用
	unsigned char **image = MakeImageStruct(x_size2,y_size2);

	// イメージ反転処理
	for(int y = 0; y < y_size2; y++){
		for(int x = 0; x < x_size2;x++){
			image[y][x] = (unsigned char)(MAX_BRIGHTNESS - image1[y][x]);
		}
	}
	
	// イメージをメンバ変数に格納する
	image2 = image;

	return OK;
}

// ヒストグラムイメージ作成
int XImage::MakeHistgramImage(){
	int ErrorCode = OK;
	long int Histgram[GRAYLEVEL];	// ヒストグラム作成用配列
	long int MaxFrequency;		// 頻度の最大値
	int data;			// 頻度を表すグラフの長さ
	
	// 元画像のヒストグラム作成
	for(int i = 0; i < GRAYLEVEL; i++){
		Histgram[i] = 0;
	}
	for(int y = 0; y < y_size1; y++){
		for(int x = 0; x < x_size1;x++){
			Histgram[image1[y][x]]++;
		}
	}

	MaxFrequency = INT_MIN;

	for(int i = 0; i < GRAYLEVEL; i++){
		if(Histgram[i] > MaxFrequency){
			MaxFrequency = Histgram[i];
		}
	}

	x_size2 = MAX_HISTGRAMSIZE;
	y_size2 = MAX_HISTGRAMSIZE;

	// 出力用イメージ画像格納用
	unsigned char **image = MakeImageStruct(x_size2,y_size2);

	for(int y = 0; y < y_size2; y++){
		for(int x = 0;x < x_size2; x++){
			image[y][x] = 0;
		}
	}
	for(int i = 0;i < GRAYLEVEL; i++){
		data = (int)(MAX_BRIGHTNESS / (double)MaxFrequency*(double)Histgram[i]);
		for(int j = 0;j < data;j++){
			image[MAX_HISTGRAMSIZE-1-j][i] = MAX_BRIGHTNESS;
		}
	}

	image2 = image;

	return ErrorCode;

}

int XImage::LinerTransformation(){
	int ErrorCode = OK;
	int min,max;		// 階調値の最大値、最小値

	// 階調値の最大値、最小値を求める
	min = INT_MAX;
	max = INT_MIN;

	for(int y = 0; y < y_size1; y++){
		for(int x = 0; x < x_size1; x++){
			if(image1[y][x] < min){
				min = image1[y][x];
			}
			if(image1[y][x] > max){
				max = image1[y][x];
			}
		}
	}

	// イメージ出力
	x_size2 = x_size1;
	y_size2 = y_size1;
	
	// 出力用イメージ画像格納用
	unsigned char **image = MakeImageStruct(x_size2,y_size2);
	for(int y = 0; y < y_size2; y++){
		for(int x = 0;x < x_size2; x++){
			image[y][x] = (unsigned char)((image1[y][x] - min) * MAX_BRIGHTNESS / (double)(max - min));
		}
	}

	image2 = image;

	return ErrorCode;
}

int XImage::SmoothImage(int GrayLevel){
	int ErrorCode = OK;
	long int hist1[GRAYLEVEL],hist2[GRAYLEVEL];
	int TransTable[GRAYLEVEL];	// 濃度変換表
	long int TargetValue;		// 変換後の頻度の目標値
	int gray;			// 階調用作業変数
	double GrayStep;		// 表現階調間隔

	// 元画像のヒストグラム作成
	for(int i = 0; i < GRAYLEVEL; i++){
		for(int y = 0; y < y_size1; y++){
			for(int x = 0; x < x_size1; x++){
				hist1[image1[y][x]]++;
			}
		}
	}

	// ヒストグラム変換表の作成
	for(int i = 0; i < GRAYLEVEL; i++){
		if(abs(TargetValue - hist2[gray]) < abs(TargetValue - (hist2[gray] + hist1[i]))){
			gray++;
			if(gray >= GrayLevel){
				gray = GrayLevel -1;
			}
		}
		TransTable[i] = gray;
		hist2[gray] = hist2[gray] + hist1[i];
	}

	// ヒストグラムを平滑化したイメージを出力する
	x_size2 = x_size1;
	y_size2 = y_size1;
	// 出力用イメージ画像格納用
	unsigned char **image = MakeImageStruct(x_size2,y_size2);

	GrayStep = (double)GRAYLEVEL / GrayLevel;
	for(int y = 0; y < y_size2; y++){
		for(int x = 0;x < x_size2; x++){
			image[y][x] = (unsigned char)(TransTable[image1[y][x]]*GrayStep);
		}
	}

	image2 = image;

	return ErrorCode;
	

}
