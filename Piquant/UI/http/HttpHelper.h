#pragma once
#include <string>
#include <Afx.h>
#include <regex>
#include "../../../libcurl/include/curl/curl.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "libcurl_a_debug.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Normaliz.lib")

namespace data_http
{
    class CHttpHelper
    {
    public:
        CHttpHelper(void);
        ~CHttpHelper(void);
    public:
        // ȫ�ֳ�ʼ��:���̻߳��� Ӧ�� �������߳��е���һ��
        bool GlobalInit(long flags = CURL_GLOBAL_WIN32);
        // ��ʼ�� easyhandle
        bool EasyInit(void);
        // ����
        bool EasySetopt(CURLoption option, void* parameter);
        // ����URL
        bool SetURL(char const * const szURL);
        // ��ѯ:֮ǰ��������URL,����SetURL
        bool Get(std::string& strResult, long* length = nullptr);
    public:
        // ��ѯ
        bool Get(std::string& strURL, std::string& strResult, long* length = nullptr);

    private:
        // ����
        void EasyCleanup(void);
        void GlobalCleanup(void);

        CURL* m_pCurl;
    };
};

