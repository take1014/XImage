#ifndef __DLL__H
#define __DLL__H

#ifdef __cplusplus
#define DLL_EXPORT extern "C"__declspec(dllexport)__stdcall
#define DLL_IMPORT extern "C"__declspec(dllimport)__stdcall
#else
#define DLL_EXPORT extern __declspec(dllexport)__stdcall
#define DLL_IMPORT extern __declspec(dllimport)__stdcall
#endif

// �N���X�G�N�X�|�[�g�p
#define DLL_CEXPORT __declspec(dllexport)
// �N���X�C���|�[�g�p
#define DLL_CIMPORT __declspec(dllimport)

#endif
