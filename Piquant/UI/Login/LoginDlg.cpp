// TestCtp01Dlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "LoginDlg.h"
#include "../../ServiceLayer/IService.h"
#include "../../ServiceLayer/CtpService.h"
#include "../../ServiceLayer/ServerInfo.h"//�洢��������Ϣ
#include <atlconv.h> //��C++�����ַ���stringת��ΪC�����ַ���
#define LAST_LOGIN_USER  "last_login_user"
#include "../../Tool/ConfigReader.h"//��ʹ��ConfigReader.h�е�get_config_reader()��Ԫ�����Զ�ȡxml��������
#include "../GlobalMgr.h"
#include "../../ServiceLayer/CtpMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLoginDlg �Ի���
CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_strusername(_T(""))
	, m_strpassword(_T(""))
	, m_bSaveUserName(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strConfigfile = "./users.ini";//�ڹ��캯���г�ʼ��ini�����ļ���
	m_storepassword = true;
	//IService* ctp_service = new CTPService();//��װ���е�ctp�ӿ�,���ж�ctp�ĵ���ȫ��ʹ��CTPService����������
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtUsername);
	DDX_Control(pDX, IDC_EDIT2, m_edtPassword);
	DDX_Control(pDX, IDC_COMBO1, m_cbServers);
	DDX_Text(pDX, IDC_EDIT1, m_strusername);
	DDX_Text(pDX, IDC_EDIT2, m_strpassword);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveUserName);
	DDX_Control(pDX, IDC_PROGRESS1, m_pbLoginProgress);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedExitApp)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_CHECK1, &CLoginDlg::OnBnClickedStorePassword)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CLoginDlg::OnLoginProgress)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);			// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	get_CtpMsgDistributor()->AddReceiver(this);//thisָ��CLoginDlg��������ؼ̳���IServiceReceiver�����������ָ����Ը�������ָ��

	
	//****************************************��ȡini�����ļ�,���ڻ�ȡ�˻�������************************************************
	//�ڵ�ǰĿ¼��ȡini�����ļ���Create�Ǿ�̬����������ʹ�ã���ֱ������::��̬��������
	std::string user_name = CIniFile::GetValue("username", LAST_LOGIN_USER, m_strConfigfile);//��ini�ļ��ж�ȡ����¼���û���
	std::string password = CIniFile::GetValue("password", LAST_LOGIN_USER, m_strConfigfile);//��ini�ļ��ж�ȡ����¼���û�����
	m_strusername = CString(user_name.c_str());	//����ini�ļ��ж�ȡ�����û���������Ӧ�Ŀؼ�����
	m_strpassword = CString(password.c_str());		//����ini�ļ��ж�ȡ�����û����븳����Ӧ�Ŀؼ�����
	UpdateData(FALSE);											//��OnInitDialog()ʱ�����ؼ�������ֵˢ�µ��ؼ�������ʽ��
	
	//****************************************��ȡxml�����ļ�,���ڻ�ȡ������************************************************
	if (!get_config_reader().load("brokers.xml", "config.xml")) //����ȡxml�����ļ�ʧ��
	{//"C:\\Users\\Quant\\Desktop\\CTP_MFC\\Project\\TestCtp01\\Debug\\brokers.xml"Ϊxml�����ļ��ľ���·��
		MessageBox(_T("��ȡ�����ļ�ʧ��"),_T("����"),MB_OK);
	}
	map<string, ServerInfo>& broker_server = get_config_reader().GetBrokers();//ͨ��GetBrokers()���нӿڻ�ȡ��������Ϣ
	map<string, ServerInfo>::iterator svr_iter = broker_server.begin();//����������
	USES_CONVERSION;
	for (; svr_iter != broker_server.end(); ++svr_iter)
	{
		m_cbServers.AddString(A2T(svr_iter->first.c_str()));//first��ȡ����map�ļ�
		m_cbServers.SetCurSel(1);//Ĭ��ѡ��
	}

	//Ĭ�ϸ�ѡ��ѡ
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻�TRUE
}


void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLoginDlg::OnBnClickedExitApp()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	exit(0);			//ʹ����ǿ���˳�
}


void CLoginDlg::OnBnClickedLogin()//�����¼��ť��Ĵ���
{
	//**********��һ�ֻ�ȡ�ؼ����ݵķ�ʽ�Ƚ�ԭʼ����Ҫ��ȡ�ؼ��ľ��������Ϊ�������ܻ�ȡ���൱�鷳**********//
	/*
	//��ȡ�û���
	TCHAR user_name[128];//�洢�û���
	HWND user_name_hwnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_EDIT1);
	::GetWindowText(user_name_hwnd, user_name, 128);
	//��ȡ�û�����
	TCHAR password[128];//�洢�û�����
	HWND password_hwnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_EDIT2);
	::GetWindowText(password_hwnd, password, 128);
	MessageBox(user_name,_T("�û���"), MB_OK);//������
	MessageBox(password, _T("�û�����"), MB_OK);//������
	*/


	//**********�ڶ��ֻ�ȡ�ؼ����ݵķ�ʽ��ͨ��ѡ���ؼ�����ӱ�������ʵ�ֱ�����ؼ���İ󶨼����ݽ����������Ϳ��Աܿ��Ȼ�þ����һ�������ӷ���**********//
	/*
	//�˴���ͨ���ؼ�������ṩ�ĺ�����ȡ���ݣ��ؼ������ָCEdit��CComboBox����ؼ������
	CString user_name, password, server_addr;
	m_edtUsername.GetWindowText(user_name);
	m_edtPassword.GetWindowText(password);
	m_cbServers.GetWindowText(server_addr);
	CString login_param = _T("�û���") + user_name + _T("�����룺") + password + _T("����¼��") + server_addr;
	MessageBox(login_param,_T("��¼��Ϣ"),MB_OK);
	*/

	//**********��������ͨ����ֵͨ������UpdateData(TRUE)���ѿؼ��е����ݷŵ�ֵ�����У��ɴ˻�ȡ��ֵ����ָCString��int����󶨿ؼ��ı���**********//
	/*
	//UpdateData(TRUE):�ѿؼ��е����ݷŵ�ֵ�����У��ɴ˻�ȡ;
	//UpdateData(FALSE):��ֵ����������ݷŵ��ؼ��ﲢ�Ҹ��¿ؼ�����ʾ��
	UpdateData(TRUE);
	CString login_param = _T("�û���") + m_strusername + _T("�����룺") + m_strpassword + _T("����¼") ;
	MessageBox(login_param, _T("��¼��Ϣ"), MB_OK);
	*/
	
	int cur_sel = m_cbServers.GetCurSel();//��ȡ��������Ͽ��е�ѡ����
	if (CB_ERR == cur_sel)
	{
		return;//���һ�û��ѡ�еĻ�������ֱ�ӷ���
	}
	CString str_sel_broker;//����ѡ�еľ�������������map<string, ServerInfo>�еĵ�һ��string����ѡ��
	m_cbServers.GetLBText(cur_sel,str_sel_broker);//��ѡ������ı�����str_sel_broker����
	USES_CONVERSION;
	string broker_name = T2A(str_sel_broker);
	map<string, ServerInfo>& broker_server = get_config_reader().GetBrokers();	//��ȡ�˴�xml�ĵ��ж�ȡ�ľ�������Ϣ
	map<string, ServerInfo>::iterator find_iter = broker_server.find(broker_name); //��broker_server���map�и���ѡ����broker_name�ҵ���Ӧ�ľ�������Ϣ
	if (broker_server.end() != find_iter)//ͨ�����Ƿ���ڳ�β�����������Ƿ��ҵ���
	{
		UpdateData(TRUE);//����Ҫˢ�¿ؼ��е����ݵ��ؼ�������
		if (get_global_mgr().GetService()->islogined()==false)
		{//��ֹ�ظ���¼
			get_global_mgr().GetService()->login(T2A(m_strusername), T2A(m_strpassword), find_iter->second);
		}
		else
		{
			::MessageBox(NULL, _T("�Ѿ���¼���Ѿ������¼"), _T("����"), MB_OK);
			return;
		}
	}
}


void CLoginDlg::OnBnClickedStorePassword()//��ѡ��ѡ�¼�
{
	// TODO: Add your control notification handler code here
	CButton *pCheckbox = (CButton*)GetDlgItem(IDC_CHECK1);
	if (pCheckbox->GetCheck())
	{//Ҫ���ס����
		m_storepassword = true;
	}
	else
	{//����ס����
		m_storepassword = false;
	}
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnReceiveData(WPARAM wParam, LPARAM lParam)//���յ�����Ϣ��wParam��lParam��
{
	if (wParam == ctp_msg::RspFrontConnected)
	{
		::PostMessage(m_pbLoginProgress.GetSafeHwnd(), PBM_SETPOS, 50, 0);//����������PBM_SETPOS��Ϣ��50%�Ľ��ȣ���Ϊֻ��ǰ�����ӳɹ�����δ��¼�ɹ�
	}
	//m_pbLoginProgress.SetPos(50);
	if (wParam == ctp_msg::RspUserLogined)//����¼�ɹ���,���ǻ������رյ�¼����
	{
		//�˴�Ҫע�⣬һ����¼�Ի��򱻹رգ�����˶Ի�����ص�����󽫱���������ʱ������Ϣ���봫���������������access-violation-reading-location�ķǷ����ʴ���
		get_CtpMsgDistributor()->DelReceiver(this);//�����ٶԻ���֮ǰ,Ӧ�Ƚ������Ϣ��������receivers���޳�����ֹ�����ٽ���Ϣ���ݹ��������ǸöԻ����ѱ�����
		PostMessage(WM_CLOSE, 0, 0);
	}
}

void CLoginDlg::OnLoginProgress(NMHDR *pNMHDR, LRESULT *pResult)
{//�˴��ǽ������¼�
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CLoginDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);						//���ؼ��е�ֵˢ�µ��󶨵Ŀؼ�������ȥ
	CIniFile::Create(m_strConfigfile);	//�ڵ�ǰĿ¼����ini�����ļ���Create�Ǿ�̬��������ʹ������::��̬������
	CIniFile::AddSection(LAST_LOGIN_USER, m_strConfigfile);		//���һ����section
	USES_CONVERSION;	//ʹ��atlconv.h��T2A����ʵ�ִ�string��char���͵�ת����
	CIniFile::SetValue("username", T2A(m_strusername), LAST_LOGIN_USER, m_strConfigfile);	//T2A��ָ��TCHARת��ΪCHAR����
	if (m_storepassword==true)
	{//��������
		CIniFile::SetValue("password", T2A(m_strpassword), LAST_LOGIN_USER, m_strConfigfile);
	}
	if (m_storepassword == false)
	{//����������
		if (CIniFile::RecordExists("password", LAST_LOGIN_USER, m_strConfigfile) == true)
		{
			CIniFile::DeleteRecord("password", LAST_LOGIN_USER, m_strConfigfile);
		}
		else
		{

		}
	}
	get_CtpMsgDistributor()->DelReceiver(this);
	__super::OnClose();
}

