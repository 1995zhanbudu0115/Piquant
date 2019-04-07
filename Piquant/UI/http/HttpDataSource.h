#pragma once
#include "HttpDataEntry.h"

namespace data_http
{
    class CHttpHelper;

    class CHttpDataSource
    {
    public:
        CHttpDataSource(void);
        ~CHttpDataSource(void);
    private:
        // ��ʼ��
        bool Init(void);

        // ������Դ
        void Release(void);

        // Http��ѯ����
        CHttpHelper* m_http;

        // ��ѯʵʱ��Ϣ  URL, sina
        static const std::string m_strQueryURL;
        static const std::string m_strQueryURLGif;
    public:
        // ��ѯʵʱ����
        bool QueryStockRealTimeData(const std::string& stockCode, CHttpDataEntry& data);
        // ���ط���K��ͼ
        bool QueryStockKLineMin(const std::string& stockCode, const std::string& filename);
    };
}
