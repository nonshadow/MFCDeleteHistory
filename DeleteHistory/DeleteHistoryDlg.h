
// DeleteHistoryDlg.h: 头文件
//

#pragma once
#include <string>

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
	CDateTimeCtrl m_DateEndCtrl;	// 日期时间控件
	CButton m_BtnStartCtrl;			// 开始按钮控件 
	CProgressCtrl m_ProgRunCtrl;	// 运行进度条控件
public:
	//CString 字符串 转 string 字符串
	std::string CStringToString(CString _inPath);
	void OutPutLog(CString _log);

	CEdit m_LogCtrl;
};
