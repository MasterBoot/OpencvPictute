
// OpencvPictuteDlg.h : ͷ�ļ�
//
#include "cv.h" 
#include "highgui.h"
#pragma once


// COpencvPictuteDlg �Ի���
class COpencvPictuteDlg : public CDialogEx
{
// ����
public:
	COpencvPictuteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OPENCVPICTUTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void detect_and_draw(IplImage* image);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
};
