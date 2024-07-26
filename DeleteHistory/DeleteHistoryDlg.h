
// DeleteHistoryDlg.h: 头文件
//

#pragma once
#include <string>
#include <queue>
#include <io.h>
#include <direct.h>

// CDeleteHistoryDlg 对话框
class CDeleteHistoryDlg : public CDialogEx
{
// 构造
public:
	CDeleteHistoryDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETEHISTORY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CString m_Title;					// 程序主窗口标题
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_FilePath;				// 文件目录文本
	std::string strPath;			// 文件目录文本 string格式
	CString m_LogTxt;				// 运行日志文本
	CTime timeStop;					// 截至日期
	long long dTime = 0;			// 日期时间戳
	int retFlag = 0;				// 线程状态标志
	UINT fileNum = 0;				// 检索到文件个数
	UINT delNum = 0;				// 清理计数
	BOOL b_EndDate;					// 截至日期模式选择控件 状态
	BOOL b_PauseBtn;				// 暂停按钮选择状态
	CWinThread* m_pThread;			// 工作线程指针

	CDateTimeCtrl m_DateEndCtrl;	// 日期时间控件
	CButton m_BtnStartCtrl;			// 开始按钮控件 
	CProgressCtrl m_ProgRunCtrl;	// 运行进度条控件
	CEdit m_LogCtrl;				// 运行日志控件
	CButton m_CheckEndDate;			// 截至日期模式按钮控件
	CButton m_CheckBtnPuaseCtrl;	// 暂停按钮控件

public:
	// CString 字符串 转 string 字符串
	std::string CStringToString(CString _inPath);
	// 日志打印 bool _Flag = TRUE;			// 日志输出判断标志，true 保留，false 循环清除
	void OutputLog(CString _log);
	// 获取目标文件个数
	int GetFileNum(std::string _inPath, long _timeInfo);
	// 删除目标文件及空文件夹
	void DeleteFolder(std::string _inPath, long _timeInfo);
	
	// 静态线程函数
	static UINT __cdecl AfxStartFunc(void* arg);
	// 生成的消息映射函数
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnDtnCloseupDateEnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheckEnddate();
	
	afx_msg void OnBnClickedCheckPause();
};
