
// AcdkOpenCVView.h : interface of the CAcdkOpenCVView class
//

#pragma once


#include "opencv/cv.h"				//영상 처리를 위한 Header
#include "opencv/highgui.h"			//카메라로 영상을 입력받거나 이미지를 읽어들이고 화면에 보여주기 위한 Header

class CAcdkOpenCVView : public CView
{
protected: // create from serialization only
	CAcdkOpenCVView();
	DECLARE_DYNCREATE(CAcdkOpenCVView)

// Attributes
public:
	CAcdkOpenCVDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CAcdkOpenCVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()


public:
	IplImage* contour(IplImage* img);
	IplImage *T9_Mosaic2(IplImage *img, int div);
	void Initial();
	void DrawCamPicture();
	void Exit();


	int i, threshold = 128, mosaicdiv = 1; // 임계값(Threshold) 설정
	IplImage* image = 0;
	IplImage* output = 0;
	IplImage* gray = 0;
	IplImage* cont = 0;
	CvCapture* capture;
	IplImage *resize_img;
	IplImage *mosaic_img2;
	IplImage* convert;
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // debug version in AcdkOpenCVView.cpp
inline CAcdkOpenCVDoc* CAcdkOpenCVView::GetDocument() const
   { return reinterpret_cast<CAcdkOpenCVDoc*>(m_pDocument); }
#endif

