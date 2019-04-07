#pragma once
#include <vector>

// ǰ������
namespace data_http
{
    class CHttpDataEntry;
}

class CDrawBuySellPanel
{
public:
    explicit CDrawBuySellPanel();
    virtual ~CDrawBuySellPanel();
    void Update(Gdiplus::Graphics& gh, int x, int y, const data_http::CHttpDataEntry& data); //���±�����ϸ���ⲿ�ӿ�

private:
    // ���»���: �����Ļ�ͼ,�� public: Update����
    void Update(Gdiplus::Graphics& gh, int x, int y, const std::vector<std::string>& vSellBuy, const std::string& open);

    // ����Ŀ��
    HDC m_hdc;
};

