// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//����TOOL_API��,��Ϊ�˵�����dll�е��༰��Ԫ����
//_WIN32��TOOL_EXPORTS����Ԥ�����,��ͨ������ҳ�µ�C/C++�µ�preprocessor�µ�preprocessor definitions���Զ�������,��ͨ��Ԥ���������Щ���Ʋ���
#ifdef _WIN32
#ifdef TOOL_EXPORTS
#define TOOL_API _declspec(dllexport) 
#else
#define TOOL_API _declspec(dllimport)
#endif
#else
#define TOOL_API //���ڷ�win32ƽ̨��,��liunx��,��TOOL_APIΪ��
#endif

// TODO: reference additional headers your program requires here
