
// DeleteHistoryDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DeleteHistory.h"
#include "DeleteHistoryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define V 1.0

// CDeleteHistoryDlg 对话框



CDeleteHistoryDlg::CDeleteHistoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETEHISTORY_DIALOG, pParent)
	, m_FilePath(_T(""))
	, m_LogTxt(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeleteHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_FilePath);
	DDX_Text(pDX, IDC_EDIT_LOG, m_LogTxt);
	DDX_Control(pDX, IDC_DATE_END, m_DateEndCtrl);
	DDX_Control(pDX, IDC_BTN_START, m_BtnStartCtrl);
	DDX_Control(pDX, IDC_PROG_RUN, m_ProgRunCtrl);
	DDX_Control(pDX, IDC_EDIT_LOG, m_LogCtrl);
}

BEGIN_MESSAGE_MAP(CDeleteHistoryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDeleteHistoryDlg 消息处理程序

BOOL CDeleteHistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_Title.Format(_T("历史文件清理工具 v%.1f By:Nonshadow"), V);
	
	SetWindowText(m_Title);
	
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDeleteHistoryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDeleteHistoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::string CDeleteHistoryDlg::CStringToString(CString _inPath)
{
	std::string buff;
	CStringA stra(_inPath.GetBuffer(255));
	_inPath.ReleaseBuffer();
	buff = stra.GetBuffer(255);
	stra.ReleaseBuffer();
	return buff;
}

void CDeleteHistoryDlg::OutPutLog(CString _log)
{
	UpdateData();
	CString timeStr;
	CTime tim;
	tim = CTime::GetCurrentTime(0);
	timeStr = tim.Format("%Y-%m-%d %H:%M:%S  ");
	m_LogTxt += timeStr;
	m_LogTxt += _log;
	m_LogTxt += "\r\n";
	m_LogCtrl.SetDlgItemText(IDC_EDIT_LOG, m_LogTxt);
	UpdateData(FALSE);
	m_LogCtrl.LineScroll(m_LogCtrl.GetLineCount());
}

