
// OpencvPictuteDlg.h : 头文件
//
#include "cv.h" 
#include "highgui.h"
#pragma once


// COpencvPictuteDlg 对话框
class COpencvPictuteDlg : public CDialogEx
{
// 构造
public:
	COpencvPictuteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPENCVPICTUTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void detect_and_draw(IplImage* image);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedButfile();
	CString m_strFilePath;
};
