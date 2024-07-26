
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

#define V 1.2

// CDeleteHistoryDlg 对话框



CDeleteHistoryDlg::CDeleteHistoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETEHISTORY_DIALOG, pParent)
	, m_FilePath(_T(""))
	, m_LogTxt(_T(""))
	, b_EndDate(FALSE)
	, b_PauseBtn(FALSE)
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
	DDX_Control(pDX, IDC_CHECK_ENDDATE, m_CheckEndDate);
	DDX_Check(pDX, IDC_CHECK_ENDDATE, b_EndDate);
	DDX_Control(pDX, IDC_CHECK_PAUSE, m_CheckBtnPuaseCtrl);
	DDX_Check(pDX, IDC_CHECK_PAUSE, b_PauseBtn);
}

BEGIN_MESSAGE_MAP(CDeleteHistoryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CDeleteHistoryDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_START, &CDeleteHistoryDlg::OnBnClickedBtnStart)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATE_END, &CDeleteHistoryDlg::OnDtnCloseupDateEnd)
	ON_BN_CLICKED(IDC_CHECK_ENDDATE, &CDeleteHistoryDlg::OnBnClickedCheckEnddate)
	ON_BN_CLICKED(IDC_CHECK_PAUSE, &CDeleteHistoryDlg::OnBnClickedCheckPause)
END_MESSAGE_MAP()


// CDeleteHistoryDlg 消息处理程序

BOOL CDeleteHistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_Title.Format(_T("历史文件清理工具 v%.1f -- By: Nonshadow <52pojie>"), V);
	SetWindowText(m_Title);
	
	// TODO: 在此添加额外的初始化代码
	m_BtnStartCtrl.EnableWindow(FALSE);
	b_EndDate = FALSE;
	m_CheckEndDate.SetWindowText(_T("创建"));
	m_CheckBtnPuaseCtrl.EnableWindow(FALSE);
	b_PauseBtn = FALSE;
	m_CheckBtnPuaseCtrl.SetWindowText(_T("等待中"));

	UpdateData(FALSE);
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

void CDeleteHistoryDlg::OutputLog(CString _log)
{
	UpdateData();
	int n = m_LogCtrl.GetLineCount();
	if (n > 30)
	{
		m_LogCtrl.SetSel(0, -1);
		m_LogCtrl.Clear();
		m_LogTxt = "";
	}
	CString timeStr;
	CTime tim;
	tim = CTime::GetCurrentTime(0);
	timeStr = tim.Format("%Y-%m-%d %H:%M:%S  ");
	m_LogTxt += timeStr;
	m_LogTxt += _log;
	m_LogTxt += "\r\n";
	m_LogCtrl.SetDlgItemText(IDC_EDIT_LOG, _log);
	UpdateData(FALSE);
	m_LogCtrl.LineScroll(m_LogCtrl.GetLineCount());
}

int CDeleteHistoryDlg::GetFileNum(std::string _inPath, long _timeInfo)
{	
	int n = 0;				//文件计数
	std::queue<std::string> q;
	q.push(_inPath);
	while (!q.empty())
	{
		std::string item = q.front();		//获取要查找的文件夹路径
		q.pop();
		std::string path = item + "\\*";	//通配符 *  遍历该目录下所有文件及文件夹
		struct _finddata32_t fileinfo;
		auto handle = _findfirst32(path.c_str(), &fileinfo);
		if (handle == -1)
		{
			continue;
		}
		while (!_findnext32(handle, &fileinfo))
		{
			if (fileinfo.attrib & _A_SUBDIR)		//当查找到的是文件夹时
			{
				if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				{
					continue;
				}
				q.push(item + "\\" + fileinfo.name);
			}
			else
			{
				long timeBuf = 0;
				if (b_EndDate)
				{
					timeBuf = fileinfo.time_write;		// time_write   文件最后一次修改时间	
				}
				else
				{
					timeBuf = fileinfo.time_create;		// time_create   文件最后一次修改时间
				}
				if (timeBuf < _timeInfo)	
				{
					n++;
				}
				else
				{
					//continue;			//跳出本次循环，继续下一次循环，遍历其余文件
					break;				//跳出循环，不再遍历后续文件，截止到当前文件，后续不再判断
				}
			}
		}
		_findclose(handle);
	}
	return n;
}

void CDeleteHistoryDlg::DeleteFolder(std::string _inPath, long _timeInfo)
{
	std::string strFilePath = _inPath + "\\*.*";
	struct _finddata32_t fileInfo;
	auto handle = _findfirst32(strFilePath.c_str(), &fileInfo);
	if (handle == -1)
	{
		CString buf;
		buf.Format(_T("查找文件出错，错误代码：%d"), errno);
		OutputLog(buf);
		retFlag = -1;
		return;
	}
	do
	{
		if (fileInfo.attrib & _A_SUBDIR)  //如果为文件夹，加上文件夹路径再次遍历
		{
			if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
			{
				continue;
			}
			//	在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strFilePath = _inPath + "\\" + fileInfo.name;
			//	先遍历删除文件夹下的文件，再删除空的文件夹
			DeleteFolder(strFilePath.c_str(), _timeInfo);
			//	删除文件夹
			int errnoInfo = _rmdir(strFilePath.c_str());
			if (errnoInfo != 0)
			{
				CString buf(strFilePath.c_str());
				CString	err;
				err.Format(_T("错误代码：%d"), errno);
				buf += _T(" 文件夹删除失败，");
				buf += err;
				OutputLog(buf);
				retFlag = 1;
				break;
			}
		}
		else
		{
			std::string filePath = _inPath + "\\" + fileInfo.name;
			//判断文件最后一次修改日期时间戳 是否小于截至日期时间戳
			long timeBuf = 0;
			if (b_EndDate)
			{
				timeBuf = fileInfo.time_write;		// time_write   文件最后一次修改时间
			}
			else
			{
				timeBuf = fileInfo.time_create;		// time_create   文件最后一次修改时间
			}
			if (timeBuf < _timeInfo)
			{
				//	删除文件
				int ret = remove(filePath.c_str());
				if (ret == -1)
				{
					CString buf(filePath.c_str());
					CString	err;
					err.Format(_T("错误代码：%d"), errno);
					buf += _T(" 删除失败，");
					buf += err;
					OutputLog(buf);
					retFlag = 1;
					break;
				}
				delNum++;
				m_ProgRunCtrl.SetPos(delNum);
				//Sleep(1000);
				/*if (delNum % 100 == 0)
				{
					CString buff;
					buff.Format(_T("清理进行中…… 已清理 %d ，剩余 %d 。"), delNum, fileNum - delNum);
					OutputLog(buff);
				}*/
			}
			else
			{
				//continue;			//跳出本次循环，继续下一次循环，遍历其余文件
				break;		//跳出循环，不再遍历后续文件，截止到当前文件，后续不再判断
			}
		}
	} while (!_findnext32(handle, &fileInfo));
	_findclose(handle);
}

UINT __cdecl CDeleteHistoryDlg::AfxStartFunc(void* arg)
{
	CDeleteHistoryDlg* dlg = (CDeleteHistoryDlg*)arg;
	dlg->DeleteFolder(dlg->strPath, dlg->dTime);
	CString buff;
	buff.Format(_T("清理结束，已清理 %d ，剩余 %d 。"), dlg->delNum, dlg->fileNum - dlg->delNum);
	dlg->OutputLog(buff);
	dlg->m_CheckBtnPuaseCtrl.EnableWindow(FALSE);
	dlg->m_CheckBtnPuaseCtrl.SetWindowText(_T("已终止"));
	switch (dlg->retFlag)
	{
	case -1:
		dlg->OutputLog(_T("文件查找失败！请检查错误代码！"));
	case 0:
		dlg->OutputLog(_T("指定文件已清理！"));
		break;
	case 1:
		dlg->OutputLog(_T("部分文件或目录未删除，请检查相应错误代码！"));		//(搜errno 常量如：13=无权限，2=无此目录或文件……)
		break;
	default:
		break;
	}
	Sleep(100);
	dlg->OutputLog(_T("---------- 任务完成！----------"));
	dlg->OutputLog(_T("如有其他任务，请点击 “打开” 按钮选择目录！如任务结束，请点击 “退出” 按钮！"));
	return 0;
}

void CDeleteHistoryDlg::OnBnClickedBtnOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szPath[MAX_PATH] = {};
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = _T("请选择文件夹路径");
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);

	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, szPath))
		{
			CString	buff = szPath;
			m_FilePath = buff /*+_T("\\Data\\his\\1")*/;
			UpdateData(FALSE);
			strPath = CStringToString(m_FilePath);
			CString buf;
			buf = _T("待清理目录：") + m_FilePath;
			OutputLog(buf);
		}
		//使用 IMalloc 接口，避免内存泄露
		IMalloc* pMalloc;
		if (SHGetMalloc(&pMalloc) != NOERROR)
		{
			OutputLog(_T("无法获取IMalloc界面!\n"));
		}
		pMalloc->Free(pItemIDList);
		if (pMalloc)
		{
			pMalloc->Release();
		}
		m_BtnStartCtrl.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
	//获取日期控件显示的时间
	m_DateEndCtrl.GetTime(timeStop);
	//将日期时间转换为时间戳
	dTime = (long long)timeStop.GetTime();
	CString buf{};
	CString strTime = timeStop.Format("%Y-%m-%d");
	fileNum = GetFileNum(strPath, dTime);
	buf.Format(_T("检索到 %d 个符合要求的文件！"), fileNum);
	OutputLog(buf);
}

void CDeleteHistoryDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	OutputLog(_T("--------------- 清理程序 初始化 ---------------"));
	//获取日期控件显示的时间
	m_DateEndCtrl.GetTime(timeStop);
	//将日期时间转换为时间戳
	dTime = (long long)timeStop.GetTime();
	CString buf{};
	CString strTime = timeStop.Format("%Y-%m-%d");
	fileNum = GetFileNum(strPath, dTime);
	CString csn;
	csn.Format(_T("%d"), fileNum);
	buf = _T("截至 ") + strTime + _T("  之前的 ") + csn + _T(" 个历史文件将被删除！");
	UINT i;
	i = MessageBox(m_FilePath + _T(" 文件夹中，") + buf + _T("\r\n确定要删除吗？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);
	if (i == IDNO)
	{
		OutputLog(_T("清理任务已取消！"));
	}
	if (i == IDYES)
	{
		OutputLog(buf);
		OutputLog(_T("--------------- 清理程序 进行中 ---------------"));
		//初始化进度条范围
		if (fileNum == 0)
		{
			m_ProgRunCtrl.SetRange(0, 2);
		}
		m_ProgRunCtrl.SetRange(0, fileNum);
		m_ProgRunCtrl.SetPos(1);
		m_pThread = AfxBeginThread(AfxStartFunc, this);
		m_CheckBtnPuaseCtrl.EnableWindow(TRUE);
		m_CheckBtnPuaseCtrl.SetWindowText(_T("暂停"));
	}
}

void CDeleteHistoryDlg::OnDtnCloseupDateEnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//获取日期控件显示的时间
	m_DateEndCtrl.GetTime(timeStop);
	//将日期时间转换为时间戳
	dTime = (long long)timeStop.GetTime();
	CString buf{};
	CString strTime = timeStop.Format("%Y-%m-%d");
	fileNum = GetFileNum(strPath, dTime);
	buf.Format(_T("检索到 %d 个符合要求的文件！"), fileNum);
	OutputLog(buf);
}

void CDeleteHistoryDlg::OnBnClickedCheckEnddate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (b_EndDate)
	{
		b_EndDate = FALSE;
		m_CheckEndDate.SetWindowText(_T("创建"));
	}
	else
	{
		b_EndDate = TRUE;
		m_CheckEndDate.SetWindowText(_T("修改"));
	}
}

void CDeleteHistoryDlg::OnBnClickedCheckPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!b_PauseBtn)
	{
		b_PauseBtn = TRUE;
		SuspendThread(m_pThread->m_hThread);		//挂起（暂停）工作线程
		m_CheckBtnPuaseCtrl.SetWindowText(_T("继续"));
		OutputLog(_T("清理线程已暂停运行！如需恢复，请点击 “继续” 按钮！"));
	}
	else
	{
		b_PauseBtn = FALSE;
		ResumeThread(m_pThread->m_hThread);		//恢复（继续）工作线程
		m_CheckBtnPuaseCtrl.SetWindowText(_T("暂停"));
		OutputLog(_T("清理线程已恢复运行！如需暂停，请点击 “暂停” 按钮！"));
	}
}
