
#include "stdafx.h"
#include "../mainfrm.h"
#include "StrategyTree.h"
#include "../Resource.h"
#include "../App.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_DUMMY_COMPILE, StartStrategy)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_REFRESH, &CFileView::OnRefresh)
	ON_COMMAND(ID_OPENFILE, &CFileView::OnOpenfile)
	ON_COMMAND(ID_STOPSTRATEGY, &CFileView::OnStopstrategy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{//ɨ��Ŀ¼����dll;
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("���Խӿ�"), 0, 0);	//���Ը����
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	CFileFind file_find;
	BOOL bFind = file_find.FindFile(_T("*.dll"));  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
	while (bFind)
	{
		bFind = file_find.FindNextFile();
		CString DataFile = file_find.GetFilePath(); //GetFilePath���õ�ȫ·����;
		HTREEITEM hSrc = hRoot;
		if (file_find.IsDirectory() && !file_find.IsDots())    //�����Ŀ¼ 
		{
			hSrc = m_wndFileView.InsertItem(file_find.GetFileName(), 0, 0, hRoot);
		}
		else if (!file_find.IsDirectory() && !file_find.IsDots())//��ΪĿ¼
		{
			CString FileTitle = file_find.GetFileTitle();//GetFileTitle:�õ�������׺�ļ���
			CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
			HMODULE hModule = LoadLibrary(file_find.GetFilePath());
			if (hModule == NULL)
			{
				//��Ϊ���ļ�����һ��dll;
			}
			else
			{
				FARPROC proc = GetProcAddress(hModule, "CreateStrategy");	//����Ƿ���CreateStrategy�����ɵ�����Ψ�пɵ�����dll���ǲ����ļ�
				pfnCreateStrategy pfnCreator = (pfnCreateStrategy)GetProcAddress(hModule, "CreateStrategy");
				if (NULL != pfnCreator)
				{
					HTREEITEM hInc = m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
					m_wndFileView.SetItemData(hInc, (DWORD_PTR)hModule);
				}
			}
			//........
		}
	}
	file_find.Close();

	HTREEITEM hRoot_Log = m_wndFileView.InsertItem(_T("��־"), 0, 0);	//��־���
	m_wndFileView.SetItemState(hRoot_Log, TVIS_BOLD, TVIS_BOLD);
	bFind = file_find.FindFile(_T("log/*.txt"));
	while (bFind)
	{
		bFind = file_find.FindNextFile();
		CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
		HTREEITEM hSrc = hRoot_Log;
		m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
	}
	file_find.Close();

	HTREEITEM hRoot_Python = m_wndFileView.InsertItem(_T("�ű�"), 0, 0);	//�ű����
	m_wndFileView.SetItemState(hRoot_Python, TVIS_BOLD, TVIS_BOLD);
	bFind = file_find.FindFile(_T("script/*.py"));
	while (bFind)
	{
		bFind = file_find.FindNextFile();
		CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
		HTREEITEM hSrc = hRoot_Python;
		m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
	}
	file_find.Close();

	HTREEITEM hRoot_Exe = m_wndFileView.InsertItem(_T("exe��ݷ�ʽ"), 0, 0);	//�������
	m_wndFileView.SetItemState(hRoot_Exe, TVIS_BOLD, TVIS_BOLD);
	bFind = file_find.FindFile(_T("lnk/*.lnk"));
	while (bFind)
	{
		bFind = file_find.FindNextFile();
		CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
		HTREEITEM hSrc = hRoot_Exe;
		m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
	}
	file_find.Close();

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hRoot_Log, TVE_EXPAND);
	m_wndFileView.Expand(hRoot_Python, TVE_EXPAND);
	m_wndFileView.Expand(hRoot_Exe, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������: 
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CFileView::StartStrategy()
{//����¼���Ӧ
	// TODO:  �ڴ˴���������������
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	CString nodetext = m_wndFileView.GetItemText(hItem);
	int length = nodetext.Find(_T(".")); //�ָ��
	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("txt") || nodetext.Right(nodetext.GetLength() - length - 1) == _T("py") || nodetext.Right(nodetext.GetLength() - length - 1) == _T("lnk"))
	{
		MessageBox(_T("�ǲ���"), _T("����"), MB_OK);
		return;
	}
	HMODULE hStrategyModule = (HMODULE)m_wndFileView.GetItemData(hItem);
	//GetProcAddress
	//pfn()
	pfnCreateStrategy pfnCreator = (pfnCreateStrategy)GetProcAddress(hStrategyModule, "CreateStrategy"); //pfnCreateStrateyΪ����ָ��,��ȡ��������
	if (pfnCreator)
	{
		IStrategy* pIStrategy = reinterpret_cast<IStrategy*>(pfnCreator()); //pfnCreator()�˺���ִ�к�õ����Կ����
		if (NULL == pIStrategy)
		{
			//AfxMessageBox(_T("���Դ���ʧ��...."));
			MessageBox(_T("���Դ���ʧ��"), _T("����"), MB_OK);
		}
		else
		{
			if (pIStrategy->isRunning()==true)
			{//�Ѿ�����������,�ܾ��ٴ���������
				MessageBox(_T("����������"), _T("����"), MB_OK);
			}
			else
			{
				//AfxMessageBox(_T("���Դ����ɹ�...."));
				pIStrategy->set_ApiInterface(get_global_mgr().GetService());	//Ĭ�϶Խ�ctp,���Ժ�Խ�IB�������ӿڣ����
				pIStrategy->start();	//������������
			}
		}
	}
}

void CFileView::OnStopstrategy()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	CString nodetext = m_wndFileView.GetItemText(hItem);
	int length = nodetext.Find(_T(".")); //�ָ��
	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("txt") || nodetext.Right(nodetext.GetLength() - length - 1) == _T("py") || nodetext.Right(nodetext.GetLength() - length - 1) == _T("lnk"))
	{
		MessageBox(_T("�ǲ���"), _T("����"), MB_OK);
		return;
	}
	HMODULE hStrategyModule = (HMODULE)m_wndFileView.GetItemData(hItem);
	pfnCreateStrategy pfnCreator = (pfnCreateStrategy)GetProcAddress(hStrategyModule, "CreateStrategy"); //pfnCreateStrateyΪ����ָ��,��ȡ��������
	if (pfnCreator)
	{
		IStrategy* pIStrategy = reinterpret_cast<IStrategy*>(pfnCreator()); //pfnCreator()�˺���ִ�к�õ����Կ����
		if (NULL == pIStrategy)
		{
			//AfxMessageBox(_T("����ֹͣʧ��...."));
			MessageBox(_T("����ֹͣʧ��"), _T("����"), MB_OK);
		}
		else
		{
			//AfxMessageBox(_T("����ֹͣ�ɹ�...."));
			if (pIStrategy->isRunning()==false)
			{//�Ѿ�ֹͣ������,�ܾ��ٴ�����ֹͣ����
				MessageBox(_T("������δ����/�����Ѿ�ֹͣ"), _T("����"), MB_OK);
			}
			else
			{
				pIStrategy->stop();	//ֹͣ��������,ע��ֻ����ͣ����
			}
		}
	}
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ:  %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

BOOL CFileView::CanBeClosed() const
{
	return FALSE;
}

BOOL CFileView::FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod /*= DM_UNKNOWN*/, bool bShow /*= true*/)
{
	return true;
}



void CFileView::OnRefresh()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	CString nodetext = m_wndFileView.GetItemText(hItem);
	int length = nodetext.Find(_T(".")); //�ָ��
	//���ݲ�ͬ���ڵ�������ˢ��
	bool bFind;
	CFileFind file_find;

	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("txt"))
	{
		HTREEITEM hSrc = m_wndFileView.GetParentItem(hItem); //���������ֻ���ڶ������ṹ���˴����ڻ�ȡһ�������
		HTREEITEM hCurItem = m_wndFileView.GetChildItem(hSrc); //��һ����㿪ʼ����ȡ�����ӽ��
		HTREEITEM hPreItem;
		while (hCurItem)
		{
			hPreItem = hCurItem; //��¼Ϊ��һ���
			hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
			m_wndFileView.DeleteItem(hPreItem);
		}
		bFind = file_find.FindFile(_T("log/*.txt"));
		while (bFind)
		{
			bFind = file_find.FindNextFile();
			CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
			//ˢ��ǰ����������ӽ��
			m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
		}
		file_find.Close();
	}
	else if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("py"))
	{
		//ˢ��ǰ����������ж������
		HTREEITEM hSrc = m_wndFileView.GetParentItem(hItem); //���������ֻ���ڶ������ṹ���˴����ڻ�ȡһ�������
		HTREEITEM hCurItem = m_wndFileView.GetChildItem(hSrc); //��һ����㿪ʼ����ȡ�����ӽ��
		HTREEITEM hPreItem;
		while (hCurItem)
		{
			hPreItem = hCurItem; //��¼Ϊ��һ���
			hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
			m_wndFileView.DeleteItem(hPreItem);
		}
		bFind = file_find.FindFile(_T("script/*.py"));
		while (bFind)
		{
			bFind = file_find.FindNextFile();
			CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
			m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
		}
		file_find.Close();
	}
	else if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("lnk"))
	{
		//ˢ��ǰ����������ж������
		HTREEITEM hSrc = m_wndFileView.GetParentItem(hItem); //���������ֻ���ڶ������ṹ���˴����ڻ�ȡһ�������
		HTREEITEM hCurItem = m_wndFileView.GetChildItem(hSrc); //��һ����㿪ʼ����ȡ�����ӽ��
		HTREEITEM hPreItem;
		while (hCurItem)
		{
			hPreItem = hCurItem; //��¼Ϊ��һ���
			hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
			m_wndFileView.DeleteItem(hPreItem);
		}
		bFind = file_find.FindFile(_T("lnk/*.lnk"));
		while (bFind)
		{
			bFind = file_find.FindNextFile();
			CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
			m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
		}
		file_find.Close();
	}
	else if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("dll"))
	{
		//ˢ��ǰ����������ж������
		HTREEITEM hSrc = m_wndFileView.GetParentItem(hItem); //���������ֻ���ڶ������ṹ���˴����ڻ�ȡһ�������
		HTREEITEM hCurItem = m_wndFileView.GetChildItem(hSrc); //��һ����㿪ʼ����ȡ�����ӽ��
		HTREEITEM hPreItem;
		while (hCurItem)
		{
			hPreItem = hCurItem; //��¼Ϊ��һ���
			hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
			m_wndFileView.DeleteItem(hPreItem);
		}
		BOOL bFind = file_find.FindFile(_T("*.dll"));  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
		while (bFind)
		{
			bFind = file_find.FindNextFile();
			if (file_find.IsDirectory() && !file_find.IsDots())    //�����Ŀ¼ 
			{
				continue;
			}
			else if (!file_find.IsDirectory() && !file_find.IsDots())//��ΪĿ¼
			{
				CString FileTitle = file_find.GetFileTitle();//GetFileTitle:�õ�������׺�ļ���
				CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
				HMODULE hModule = LoadLibrary(file_find.GetFilePath());
				if (hModule == NULL)
				{
					//��Ϊ���ļ�����һ��dll;
					continue;
				}
				else
				{
					FARPROC proc = GetProcAddress(hModule, "CreateStrategy");	//����Ƿ���CreateStrategy�����ɵ�����Ψ�пɵ�����dll���ǲ����ļ�
					pfnCreateStrategy pfnCreator = (pfnCreateStrategy)GetProcAddress(hModule, "CreateStrategy");
					if (NULL != pfnCreator)
					{
						HTREEITEM hInc = m_wndFileView.InsertItem(FileName, 1, 1, hSrc);
						m_wndFileView.SetItemData(hInc, (DWORD_PTR)hModule);
					}
				}
			}
		}
		file_find.Close();
	}
	else//������ѡ����������µ�ˢ�£�����ѡ��һ������µ�ˢ��
	{
		if (nodetext == _T("���Խӿ�"))
		{
			//ˢ��ǰ����������ж������
			HTREEITEM hCurItem = m_wndFileView.GetChildItem(hItem); //��һ����㿪ʼ����ȡ�����ӽ��
			HTREEITEM hPreItem;
			while (hCurItem)
			{
				hPreItem = hCurItem; //��¼Ϊ��һ���
				hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
				m_wndFileView.DeleteItem(hPreItem);
			}
			BOOL bFind = file_find.FindFile(_T("*.dll"));  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
			while (bFind)
			{
				bFind = file_find.FindNextFile();
				if (file_find.IsDirectory() && !file_find.IsDots())    //�����Ŀ¼ 
				{
					continue;
				}
				else if (!file_find.IsDirectory() && !file_find.IsDots())//��ΪĿ¼
				{
					CString FileTitle = file_find.GetFileTitle();//GetFileTitle:�õ�������׺�ļ���
					CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
					HMODULE hModule = LoadLibrary(file_find.GetFilePath());
					if (hModule == NULL)
					{
						//��Ϊ���ļ�����һ��dll;
						continue;
					}
					else
					{
						FARPROC proc = GetProcAddress(hModule, "CreateStrategy");	//����Ƿ���CreateStrategy�����ɵ�����Ψ�пɵ�����dll���ǲ����ļ�
						pfnCreateStrategy pfnCreator = (pfnCreateStrategy)GetProcAddress(hModule, "CreateStrategy");
						if (NULL != pfnCreator)
						{
							HTREEITEM hInc = m_wndFileView.InsertItem(FileName, 1, 1, hItem);
							m_wndFileView.SetItemData(hInc, (DWORD_PTR)hModule);
						}
					}
				}
			}
			file_find.Close();
		}
		else if (nodetext == _T("��־"))
		{
			HTREEITEM hCurItem = m_wndFileView.GetChildItem(hItem); //��һ����㿪ʼ����ȡ�����ӽ��
			HTREEITEM hPreItem;
			while (hCurItem)
			{
				hPreItem = hCurItem; //��¼Ϊ��һ���
				hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
				m_wndFileView.DeleteItem(hPreItem);
			}
			bFind = file_find.FindFile(_T("log/*.txt"));
			while (bFind)
			{
				bFind = file_find.FindNextFile();
				CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
				//ˢ��ǰ����������ӽ��
				m_wndFileView.InsertItem(FileName, 1, 1, hItem);
			}
			file_find.Close();
		}
		else if (nodetext == _T("�ű�"))
		{
			//ˢ��ǰ����������ж������
			HTREEITEM hCurItem = m_wndFileView.GetChildItem(hItem); //��һ����㿪ʼ����ȡ�����ӽ��
			HTREEITEM hPreItem;
			while (hCurItem)
			{
				hPreItem = hCurItem; //��¼Ϊ��һ���
				hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
				m_wndFileView.DeleteItem(hPreItem);
			}
			bFind = file_find.FindFile(_T("script/*.py"));
			while (bFind)
			{
				bFind = file_find.FindNextFile();
				CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
				m_wndFileView.InsertItem(FileName, 1, 1, hItem);
			}
			file_find.Close();
		}
		else if (nodetext == _T("exe��ݷ�ʽ"))
		{
			//ˢ��ǰ����������ж������
			HTREEITEM hCurItem = m_wndFileView.GetChildItem(hItem); //��һ����㿪ʼ����ȡ�����ӽ��
			HTREEITEM hPreItem;
			while (hCurItem)
			{
				hPreItem = hCurItem; //��¼Ϊ��һ���
				hCurItem = m_wndFileView.GetNextSiblingItem(hCurItem);	//��Ѱ��һ�ֵܽ��Ϊ��ǰ���
				m_wndFileView.DeleteItem(hPreItem);
			}
			bFind = file_find.FindFile(_T("lnk/*.lnk"));
			while (bFind)
			{
				bFind = file_find.FindNextFile();
				CString FileName = file_find.GetFileName(); //GetFileName:�õ�����׺���ļ���
				m_wndFileView.InsertItem(FileName, 1, 1, hItem);
			}
			file_find.Close();
		}
		else
		{
			//�������ʹ���
		}
	}
}


void CFileView::OnOpenfile()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	CString nodetext = m_wndFileView.GetItemText(hItem);
	int length = nodetext.Find(_T(".")); //�ָ��
	bool bFind;
	CFileFind file_find;
	CString FilePath;

	//����ͬ�ļ����͵Ĵ�
	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("txt"))
	{
		CString fileName = _T("log//") + nodetext;
		BOOL bFind = file_find.FindFile(fileName);  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
		if (bFind)
		{
			bFind = file_find.FindNextFileW();	//��仰����Ҫ,������
			FilePath = file_find.GetRoot(); //��ȡ��·��
		}
		int ret = (int)ShellExecute(NULL, _T("edit"), m_wndFileView.GetItemText(hItem), NULL, FilePath, SW_SHOWNORMAL);
		if (ret < 32)//����Ƿ�򿪳ɹ�
			MessageBox(_T("�򿪳���"), _T("����"), MB_OK);
	}

	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("dll"))
	{
		MessageBox(_T("dll�����ļ��޷���"), _T("����"), MB_OK);
	}

	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("py"))
	{
		CString fileName = _T("script//") + nodetext;
		BOOL bFind = file_find.FindFile(fileName);  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
		if (bFind)
		{
			bFind = file_find.FindNextFileW();	//��仰����Ҫ,������
			FilePath = file_find.GetRoot(); //��ȡ��·��
		}
		int ret = (int)ShellExecute(NULL, _T("open"), m_wndFileView.GetItemText(hItem), NULL, FilePath, SW_SHOWNORMAL);
		if (ret < 32)//����Ƿ�򿪳ɹ�
			MessageBox(_T("�򿪳���"), _T("����"), MB_OK);
	}

	if (nodetext.Right(nodetext.GetLength() - length - 1) == _T("lnk"))
	{
		CString fileName = _T("lnk//") + nodetext;
		BOOL bFind = file_find.FindFile(fileName);  //file_findĬ�����ڹ������Ŀ¼ȥ����dll�ģ�Ҳ����C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug
		if (bFind)
		{
			bFind = file_find.FindNextFileW();	//��仰����Ҫ,������
			FilePath = file_find.GetRoot(); //��ȡ��·��
		}
		int ret = (int)ShellExecute(NULL, _T("open"), m_wndFileView.GetItemText(hItem), NULL, FilePath, SW_SHOWNORMAL);
		if (ret < 32)//����Ƿ�򿪳ɹ�
			MessageBox(_T("�򿪳���"), _T("����"), MB_OK);
	}
}


