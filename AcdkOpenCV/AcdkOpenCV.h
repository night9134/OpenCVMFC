
// AcdkOpenCV.h : main header file for the AcdkOpenCV application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAcdkOpenCVApp:
// See AcdkOpenCV.cpp for the implementation of this class
//

class CAcdkOpenCVApp : public CWinApp
{
public:
	CAcdkOpenCVApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAcdkOpenCVApp theApp;
