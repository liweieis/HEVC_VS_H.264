// TeeChartDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTeeChartDemoApp:
// �йش����ʵ�֣������ TeeChartDemo.cpp
//

class CTeeChartDemoApp : public CWinApp
{
public:
	CTeeChartDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTeeChartDemoApp theApp;