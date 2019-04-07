
#include "stdafx.h"
#include "../App.h"
#include "RealtimePriceDlg.h"
#include "afxdialogex.h"
#include "StringHelper.h"
#include "DrawKLineImagePanel.h"
#include "DrawBuySellPanel.h"

// CRealtimePriceDlg dialog

static CCriticalSection cs;
static std::string ProductCode_eastmoney = "";	//��¼��Ʒ����, �����Ƹ���
static std::string ProductCode_Sina = "";				//��¼��Ʒ����, ���˲ƾ���
static void GlobalSetStockCode(string code_1,  string code_2)
{
	cs.Lock();
	ProductCode_eastmoney = code_1; //�������ò�Ʒ���������˴����˷��ʳ�ͻ����
	ProductCode_Sina = code_2;
	cs.Unlock();
}
static std::string GlobalGetStockCode(int index)
{//index=0Ϊ�����Ƹ��ã� index=1Ϊ���˲ƾ���
	std::string code;
	cs.Lock();
	if (index==0)
	{
		code = ProductCode_eastmoney;
	}
	else
	{
		code = ProductCode_Sina;
	}
	cs.Unlock();
	return code;
}

// http ���ݲ�ѯ�߳�
namespace http_thread
{
	unsigned __stdcall ThreadRoutine(PVOID pArgs);
	bool bAlive = false;
	unsigned threadID = 0;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	HANDLE hEvent = INVALID_HANDLE_VALUE;

	// ��ʼ��ѯ�߳�
	void Start(CRealtimePriceDlg * pThis)
	{
		if (bAlive) 
		{//�߳��Ƿ��Ѿ�������
			return;
		}
		if (INVALID_HANDLE_VALUE != hEvent) 
		{//���Ϊ��
			return;
		}
		if (INVALID_HANDLE_VALUE != hThread) 
		{//�߳�IDҲΪ��
			return;
		}
		bAlive = true;
		hEvent = CreateEvent(NULL
			, FALSE //�Զ�����
			, FALSE // ��ʼ�����ź�״̬
			, NULL);
		hThread = (HANDLE)(::_beginthreadex(nullptr, 0, ThreadRoutine, pThis, 0, &threadID));
	}

	// ֹͣ��ѯ�߳�
	void Stop(DWORD dwTimeout = 1000 * 3 + 200)
	{
		if (bAlive) 
		{
			bAlive = false;
		}
		if (INVALID_HANDLE_VALUE != hEvent) 
		{
			CloseHandle(hEvent);
			hEvent = INVALID_HANDLE_VALUE;
		}
		if (INVALID_HANDLE_VALUE != hThread) 
		{
			WaitForSingleObject(hThread, dwTimeout);
			TerminateThread(hThread, 0);
			CloseHandle(hThread);
			hThread = INVALID_HANDLE_VALUE;
		}
	}

	// �����¼�,����һ�β�ѯ
	void FireEvent()
	{
		SetEvent(hEvent);
	}

	// ��ѯ�߳�:
	// ÿ3���ѯһ��(����� FireEvent), ��ѯ�ɹ�������Ϣ
	unsigned __stdcall ThreadRoutine(PVOID pArgs)
	{
		// ��ò���
		auto * pThis = (CRealtimePriceDlg*)pArgs;
		// ��ѯ��������
		data_http::CHttpDataSource http;
		// �鵽������
		data_http::CHttpDataEntry httpData;
		while (bAlive) 
		{//����߳��Ƿ��Ѿ�����
			// ÿ2��ȡһ������
			DWORD dwWait = ::WaitForSingleObject(hEvent, 3 * 1000);
			// �����ʱ,�������ź�, ����в�ѯ
			if (WAIT_TIMEOUT == dwWait || WAIT_OBJECT_0 == dwWait) 
			{
				// ƴ�Ӳ�ѯʱ�õ���URL
				std::string Code_eastMoney="";
				CString Code_eastMoney_cstr = _T("");
				std::string Code_sina = "";
				CString Code_sina_cstr = _T("");

				{
					std::string code = GlobalGetStockCode(0);//��ȡ�����Ƹ��õĲ�ƷID
					if (code.size() == 0)
					{
						continue;
					}
					Code_eastMoney = code;
					Code_eastMoney_cstr = code.c_str();

					code = GlobalGetStockCode(1);//��ȡ���˲ƾ��õĵĲ�ƷID
					if (code.size() == 0)
					{
						continue;
					}
					Code_sina = code;
					Code_sina_cstr = code.c_str();
				}
				// ƴ�Ӳ�ѯ�õ���URL
				CString strGIFPath;
				string GIFPath;
				{
					CString strTempPath;
					GetTempPath(MAX_PATH, strTempPath.GetBuffer(MAX_PATH)); //GetTempPath��API����ȡ����%Temp%�ı���ֵ��
					strTempPath.ReleaseBuffer();
					//strTempPath.AppendFormat(_T("%08d_%s_minutes.gif"), GetTickCount(), Code_eastMoney_cstr.GetString());
					strTempPath.AppendFormat(_T("%s_minutes.gif"), Code_eastMoney_cstr.GetString());
					strGIFPath = strTempPath.GetString();
					USES_CONVERSION;
					GIFPath = CW2A(strGIFPath.GetString());
				}
				//��������
				http.QueryStockKLineMin(Code_eastMoney, GIFPath);
				http.QueryStockRealTimeData(StringHelper::UnicodeToANSI(Code_sina_cstr.GetString()), httpData); //�����󵽵�����װ��httpData
				// �������ڵ�����,�ڴ��ڽ�����Ϣʱ�ͷ�
				auto p = new data_http::CHttpDataEntry(httpData);
				auto pFile = new CString(strGIFPath); //gifͼƬ·����
				pThis->PostMessage(CRealtimePriceDlg::sc_MSG_HTTP_QUERY_END, (WPARAM)p, (LPARAM)pFile);
			}
			else 
			{
				continue; // ����
			}
		}
		//__end:
		_endthreadex(0);
		return 0;
	}
}


IMPLEMENT_DYNAMIC(CRealtimePriceDlg, CDialogEx)

CRealtimePriceDlg::CRealtimePriceDlg(CString InstrumentID_eastMoneyneed, CString InstrumentID_sinaneed, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRealtimePriceDlg::IDD, pParent)
	, m_strCode(_T(""))
{
	//���캯���н��������ڻ��е�instrumentID
	USES_CONVERSION;
	this->rowInstrumentID_eastmoney = CW2A(InstrumentID_eastMoneyneed.GetString()); //���ն����Ƹ�����Ĳ�ƷID
	this->rowInstrumentID_sina = CW2A(InstrumentID_sinaneed.GetString()); //�������˲ƾ�����Ĳ�ƷID
}

CRealtimePriceDlg::~CRealtimePriceDlg()
{
}

BOOL CRealtimePriceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	std::string instrumentID_eastmoney = this->rowInstrumentID_eastmoney; //��ȡ�����Ƹ�Ʒ����
	std::string instrumentID_sina = this->rowInstrumentID_sina; //��ȡ���˲ƾ�Ʒ����
	// ����ѯ�̴߳�������
	GlobalSetStockCode(instrumentID_eastmoney, instrumentID_sina);
	// ��ʼ��ѯ�߳�(���δ����,�Ὺ���߳�)
	http_thread::Start(this);
	// ������ѯ�¼�,ʹ�߳�������ʼһ���²�ѯ
	http_thread::FireEvent();
	return TRUE;  
}

BOOL CRealtimePriceDlg::PreTranslateMessage(MSG* pMsg)
{
	// ��ֹ ESC/�س��� �Զ��˳�
	if (pMsg->message == WM_KEYFIRST) 
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE; // ���ط�0,�������Ϣ���������зַ�
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CRealtimePriceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CRealtimePriceDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
	http_thread::Stop();
	// ���ٴ���ǰ ��������
	CDialogEx::OnDestroy();
}

LRESULT CRealtimePriceDlg::OnMsgHttpQueryEnd(WPARAM wParam, LPARAM lParam)
{
	// �洢ʵʱ����
	auto httpData = (data_http::CHttpDataEntry *)wParam;
	// ���ص�K��ͼ·��
	auto strFilePath = (CString *)lParam;
	// ��������,��ɾ��ָ��
	if (httpData) 
	{
		m_httpData = *httpData;
		delete httpData;
		httpData = nullptr;
	}

	if (strFilePath) 
	{
		m_strFilePath = *strFilePath;
		delete strFilePath;
		strFilePath = nullptr;
	}
	
	// ����ͼƬ��ʾ���˴��ᴥ��OnPaint
	CRect r;
	GetDlgItem(STATIC_REALTIMEPRICEDISP)->GetWindowRect(r);
	ScreenToClient(&r);
	InvalidateRect(&r, TRUE);
	return 0;
}

BEGIN_MESSAGE_MAP(CRealtimePriceDlg, CDialogEx)
	ON_MESSAGE(sc_MSG_HTTP_QUERY_END, &CRealtimePriceDlg::OnMsgHttpQueryEnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRealtimePriceDlg message handlers


void CRealtimePriceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// ��һ��ͼƬ�ؼ���Ϊ�˱��ڵ���λ��
	CRect rc;
	GetDlgItem(STATIC_REALTIMEPRICEDISP)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	// ��ʼλ��
	const int x = rc.left;
	const int y = rc.top;
	// ���������
	if (true) //!m_httpData.buyCount01.empty()ͨ����һ��������Ƿ�������, ���вŸ��£��˴��ݲ�������
	{
		using namespace Gdiplus;
		//����һ��bmp
		Bitmap bmp(rc.Width(), rc.Height());
		//����bmp�� gh
		Graphics bmpGraphics(&bmp);
		bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
		// ��bmp���滭ͼ
		// �� K��ͼ
		static CDrawKLineImagePanel kLine;
		kLine.Update(bmpGraphics, x, y, m_strFilePath.c_str());

		// ����������,Ҳ��tick��Ƭ����
		static CDrawBuySellPanel dp;
		dp.Update(bmpGraphics, 610, y, m_httpData);	//ע�������ͼλ��

		// ����dc��gdi����
		Graphics graphics(dc.GetSafeHdc());
		CachedBitmap cachedBmp(&bmp, &graphics);
		// �� bmp �����������豸����
		graphics.DrawCachedBitmap(&cachedBmp, 0, 0);
	}
}



