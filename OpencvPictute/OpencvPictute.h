
// OpencvPictute.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COpencvPictuteApp:
// �йش����ʵ�֣������ OpencvPictute.cpp
//

class COpencvPictuteApp : public CWinApp
{
public:
	COpencvPictuteApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COpencvPictuteApp theApp;