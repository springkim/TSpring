
// TSpring.h : TSpring ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CTSpringApp:
// �� Ŭ������ ������ ���ؼ��� TSpring.cpp�� �����Ͻʽÿ�.
//

class CTSpringApp : public CWinApp
{
public:
	CTSpringApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTSpringApp theApp;
