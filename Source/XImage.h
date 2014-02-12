#ifndef __XImage_h
#define __XImage_h

#include "dlldef.h"
#pragma comment (lib,"XImage.lib")

// BMPファイル画像を操作するクラス
class DLL_CIMPORT XImage{
	private:
		unsigned char **image1;  // 画像用配列1のポインタ
		unsigned char **image2;  // 画像用配列2のポインタ
		int x_size1,y_size1;	 // イメージ1の縦横
		int x_size2,y_size2;     // イメージ2の縦横
		unsigned char** MakeImageStruct(int xsize,int ysize);	// イメージメモリの動的確保
		void DeleteImageStruct(unsigned char **image);		// イメージメモリの解放
		
	public:
		int ReadImage(char *FilePath);		// イメージファイル読取用関数
		int SaveImage(char *FilePath);		// イメージファイル出力用関数
		int InverseImage();			// イメージの反転処理を行う関数
		int MakeHistgramImage();		// ヒストグラムイメージ作成関数
		int LinerTransformation();		// ヒストグラムの線形変換を行ったイメージを出力する関数
		int SmoothImage(int GrayLevel);		// ヒストグラムを平滑化したイメージを出力する
};

#endif


