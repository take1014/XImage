#define MAX_IMAGESIZE 	  1024	// �z�肷��c���̍ő��f��
#define MAX_BRIGHTNESS     255	// �z�肷��ő�K��
#define GRAYLEVEL          256	// �z�肷��K����
#define MAX_HISTGRAMSIZE   256	// �q�X�g�O�����쐬���̃C���[�W�ő�T�C�Y
#define MAX_FILENAME       256	// �z�肷��t�@�C�����̍ő�l
#define MAX_BUFFERSIZE     256	// �z�肷��ő�o�b�t�@

// �f�B�U�s��
#define Bayer    1
#define Halftone 2
#define Spiral   3
// 1:Bayer�^�f�B�U�s��
int BayerMatrix[4][4] = {
	{ 0, 8, 2,10},
    	{12, 4,14, 6},
	{ 3,11, 1, 9},
	{15, 7,13, 5}
};

// 2:�ԓ_�^�f�B�U�s��
int HalftoneMatrix[4][4] = {
	{11, 4, 6, 9},
    	{12, 0, 2,14},
	{ 7, 8,10, 5},
	{ 3,15,13, 1}
};

// 3:�Q���^�f�B�U�s��
int SpiralMatrix[4][4] = {
	{ 6, 7, 8, 9},
    	{ 5, 0, 1,10},
	{ 4, 3, 2,11},
	{15,14,13,12}
};
