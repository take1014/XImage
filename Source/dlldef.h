#ifndef __DLL__H
#define __DLL__H

#ifdef __cplusplus
#define DLL_EXPORT extern "C"__declspec(dllexport)__stdcall
#define DLL_IMPORT extern "C"__declspec(dllimport)__stdcall
#else
#define DLL_EXPORT extern __declspec(dllexport)__stdcall
#define DLL_IMPORT extern __declspec(dllimport)__stdcall
#endif

// クラスエクスポート用
#define DLL_CEXPORT __declspec(dllexport)
// クラスインポート用
#define DLL_CIMPORT __declspec(dllimport)

#endif
