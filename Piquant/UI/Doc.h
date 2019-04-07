
// Doc.h : CDoc ��Ľӿ�
//


#pragma once
#include "../ServiceLayer/CtpService.h"
#include "MarketDataView/ConcurrentQueue.h"
#include "../ServiceLayer/DataTypes.h"
#include "../ServiceLayer/CtpMsgDistributor.h"

class CDoc : public CDocument, public CtpMsgReceiver
{
protected: // �������л�����
	CDoc();
	DECLARE_DYNCREATE(CDoc)

// ����
public:
	CFrameWnd* CreateNewWindow(CDocTemplate* pTemplate, CDocument* pDocument);
	virtual void OnReceiveData(WPARAM wParam, LPARAM lParam);

//��������
	ConcurrentQueue<MarketData> DataQueue; //�̰߳�ȫ�����ݶ���
	HANDLE  hMonitorThread;   //�����߳̾��
	DWORD  MonitorThreadID;  //�����߳�ID

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

//�����̺߳���
void MonitorThreadFunc(CDoc* pDoc);