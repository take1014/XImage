#ifndef __XImage_h
#define __XImage_h

#include "dlldef.h"


// BMP�t�@�C���摜�𑀍삷��N���X
class DLL_CEXPORT XImage{
	private:
		unsigned char **image1;  // �摜�p�z��1�̃|�C���^
		unsigned char **image2;  // �摜�p�z��2�̃|�C���^
		int x_size1,y_size1;	 // �C���[�W1�̏c��
		int x_size2,y_size2;     // �C���[�W2�̏c��
		unsigned char** MakeImageStruct(int xsize,int ysize);	// �C���[�W�������̓��I�m��
		void DeleteImageStruct(unsigned char **image);		// �C���[�W�������̉��
		
	public:
		int ReadImage(char *FilePath);		// �C���[�W�t�@�C���ǎ�p�֐�
		int SaveImage(char *FilePath);		// �C���[�W�t�@�C���o�͗p�֐�
		int InverseImage();			// �C���[�W�̔��]�������s���֐�
		int MakeHistgramImage();		// �q�X�g�O�����C���[�W�쐬�֐�
};

#endif
