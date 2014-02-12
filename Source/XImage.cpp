#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "XImage.h"
#include "_XImage.h"
#include "ErrorCode.h"
#include "XImageUtil.h"

// �C���[�W�t�@�C���ǎ��
int XImage::ReadImage(char *FilePath){
	FILE *fop;		// �t�@�C���I�[�v���p�̃|�C���^
	char buffer[MAX_BUFFERSIZE];
	int max_gray;
	// �t�@�C���̃I�[�v��
	fop = fopen(FilePath,"rb");
	// �t�@�C���̃I�[�v���Ɏ��s�����ꍇ�̓G���[
	if(fop == NULL){
		return ERR_ReadImage_FOPEN;
	}

	// �t�@�C���^�C�v�̊m�F
	fgets(buffer,MAX_BUFFERSIZE,fop);

	// �O���[�X�P�[���iraw�j�`���ȊO�̏ꍇ�̓G���[
	if(buffer[0] != 'P' || buffer[1] != '5'){
		return ERR_ReadImage_FILETYPE;
	}

	// x_size1,y_size1�̑��
	x_size1 = 0;
	y_size1 = 0;
	// #���������ꍇ�͓ǂݔ�΂��A#���Ȃ��ꍇ��
	// �c�������擾����
	while(x_size1 == 0 || y_size1 == 0){
		fgets(buffer,MAX_BUFFERSIZE,fop);
		if(buffer[0] != '#'){
			sscanf(buffer,"%d %d",&x_size1,&y_size1);
		}
	}
	
	// ���̓C���[�W�̓��I�m�ۂ��s��
	unsigned char **image =  MakeImageStruct(x_size1,y_size1);
	
	// �����o�ϐ��ɓ��I�m�ۂ����������̃A�h���X���i�[����
	image1 = image;

	// max_gray�̑��
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

	// �摜�f�[�^��ǂݍ���Ń����o�ϐ��Ɋi�[����
	for(int y = 0;y < y_size1; y++){
		for(int x = 0; x < x_size1; x ++){
		image[y][x] = (unsigned char)fgetc(fop);
		}
	}

	// �C���[�W�������o�ϐ��Ɋi�[����
	image1 = image;
	fclose(fop);

	return OK;
}

// �C���[�W�t�@�C���ۑ�
int XImage::SaveImage(char *FileName){
	FILE *fop;	// �t�@�C���I�[�v���p�̃|�C���^
	// �ۑ��p�̃t�@�C���|�C���^���J��
	fop = fopen(FileName,"wb");
	// �t�@�C�����ʎq"P5"��擪�ɏo�͂���
	fputs("P5\n",fop);
	// #�Ŏn�܂�R�����g�s
	fputs("#Created by XImage\n",fop);
	// �摜�̏c�������o�͂���
	fprintf(fop,"%d %d\n",x_size2,y_size2);
	// �ő�K���l�̏o��
	fprintf(fop,"%d\n",MAX_BRIGHTNESS);

	// �摜�f�[�^�̏o��
	for(int y = 0; y < y_size2; y++){
		for(int x = 0; x < x_size2;x++){
			fputc(image2[y][x],fop);
		}
	}
	fclose(fop);

	// �g�p�����C���[�W�̈�̉��
	DeleteImageStruct(image1);
	DeleteImageStruct(image2);

	return OK;
}

// �C���[�W�̈�̓��I�m��
unsigned char** XImage:: MakeImageStruct(int xsize,int ysize){
	// �������̓��I�m��
	unsigned char **image =(unsigned char**)malloc(sizeof(unsigned char*)*ysize);
	for(int i = 0; i < ysize; i++){
		image[i] = new unsigned char[xsize];
	}

	return image;
}

// �C���[�W�̈�̉��
void XImage::DeleteImageStruct(unsigned char **image){
	// �C���[�W�̈���
	free(image);
}

// �C���[�W���]����
int XImage::InverseImage(){
	x_size2 = x_size1;
	y_size2 = y_size1;
	
	// �o�͗p�C���[�W�摜�i�[�p
	unsigned char **image = MakeImageStruct(x_size2,y_size2);

	// �C���[�W���]����
	for(int y = 0; y < y_size2; y++){
		for(int x = 0; x < x_size2;x++){
			image[y][x] = (unsigned char)(MAX_BRIGHTNESS - image1[y][x]);
		}
	}
	
	// �C���[�W�������o�ϐ��Ɋi�[����
	image2 = image;

	return OK;
}

// �q�X�g�O�����C���[�W�쐬
int XImage::MakeHistgramImage(){
	int ErrorCode = OK;
	long int Histgram[GRAYLEVEL];	// �q�X�g�O�����쐬�p�z��
	long int MaxFrequency;		// �p�x�̍ő�l
	int data;			// �p�x��\���O���t�̒���
	
	// ���摜�̃q�X�g�O�����쐬
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

	// �o�͗p�C���[�W�摜�i�[�p
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
	int min,max;		// �K���l�̍ő�l�A�ŏ��l

	// �K���l�̍ő�l�A�ŏ��l�����߂�
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

	// �C���[�W�o��
	x_size2 = x_size1;
	y_size2 = y_size1;
	
	// �o�͗p�C���[�W�摜�i�[�p
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
	int TransTable[GRAYLEVEL];	// �Z�x�ϊ��\
	long int TargetValue;		// �ϊ���̕p�x�̖ڕW�l
	int gray;			// �K���p��ƕϐ�
	double GrayStep;		// �\���K���Ԋu

	// ���摜�̃q�X�g�O�����쐬
	for(int i = 0; i < GRAYLEVEL; i++){
		for(int y = 0; y < y_size1; y++){
			for(int x = 0; x < x_size1; x++){
				hist1[image1[y][x]]++;
			}
		}
	}

	// �q�X�g�O�����ϊ��\�̍쐬
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

	// �q�X�g�O�����𕽊��������C���[�W���o�͂���
	x_size2 = x_size1;
	y_size2 = y_size1;
	// �o�͗p�C���[�W�摜�i�[�p
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
