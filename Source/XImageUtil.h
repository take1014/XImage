#define MAX_IMAGESIZE 	  1024	// 想定する縦横の最大画素数
#define MAX_BRIGHTNESS     255	// 想定する最大階調
#define GRAYLEVEL          256	// 想定する階調数
#define MAX_HISTGRAMSIZE   256	// ヒストグラム作成時のイメージ最大サイズ
#define MAX_FILENAME       256	// 想定するファイル名の最大値
#define MAX_BUFFERSIZE     256	// 想定する最大バッファ

// ディザ行列
#define Bayer    1
#define Halftone 2
#define Spiral   3
// 1:Bayer型ディザ行列
int BayerMatrix[4][4] = {
	{ 0, 8, 2,10},
    	{12, 4,14, 6},
	{ 3,11, 1, 9},
	{15, 7,13, 5}
};

// 2:網点型ディザ行列
int HalftoneMatrix[4][4] = {
	{11, 4, 6, 9},
    	{12, 0, 2,14},
	{ 7, 8,10, 5},
	{ 3,15,13, 1}
};

// 3:渦巻型ディザ行列
int SpiralMatrix[4][4] = {
	{ 6, 7, 8, 9},
    	{ 5, 0, 1,10},
	{ 4, 3, 2,11},
	{15,14,13,12}
};
