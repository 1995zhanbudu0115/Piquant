// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//֮����Ҫ��IService.h�ж���SERVICE_API��,����Ϊ�����Ǹ���IService������������CtpService���������ͷ�ļ�,���ܹ�ʹ���ϸú�
//_WIN32��SERVICELAYER_EXPORTS����Ԥ�����,��ͨ������ҳ�µ�C/C++�µ�preprocessor�µ�preprocessor definitions���Զ�������,��ͨ��Ԥ���������Щ���Ʋ���
#ifdef _WIN32
#ifdef SERVICELAYER_EXPORTS
#define SERVICE_API _declspec(dllexport) 
#else
#define SERVICE_API _declspec(dllimport)
#endif
#else
#define SERVICE_API //���ڷ�win32ƽ̨��,��liunx��,��SERVICE_APIΪ��
#endif
//SERVICE_API����_declspec(dllexport)�����ø��������ڵ�����,����˵�������������������������,��Ϊ�����������ڸ���

// TODO: reference additional headers your program requires here
