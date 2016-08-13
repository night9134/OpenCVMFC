
// AcdkOpenCVView.cpp : implementation of the CAcdkOpenCVView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AcdkOpenCV.h"
#endif

#include "AcdkOpenCVDoc.h"
#include "AcdkOpenCVView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAcdkOpenCVView

IMPLEMENT_DYNCREATE(CAcdkOpenCVView, CView)

BEGIN_MESSAGE_MAP(CAcdkOpenCVView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CAcdkOpenCVView construction/destruction

CAcdkOpenCVView::CAcdkOpenCVView()
{
	// TODO: add construction code here

}

CAcdkOpenCVView::~CAcdkOpenCVView()
{
}

BOOL CAcdkOpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAcdkOpenCVView drawing

void CAcdkOpenCVView::OnDraw(CDC* /*pDC*/)
{
	CAcdkOpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	
	

	
}


// CAcdkOpenCVView printing

BOOL CAcdkOpenCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAcdkOpenCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAcdkOpenCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAcdkOpenCVView diagnostics

#ifdef _DEBUG
void CAcdkOpenCVView::AssertValid() const
{
	CView::AssertValid();
}

void CAcdkOpenCVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAcdkOpenCVDoc* CAcdkOpenCVView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAcdkOpenCVDoc)));
	return (CAcdkOpenCVDoc*)m_pDocument;
}
#endif //_DEBUG


// CAcdkOpenCVView message handlers


int CAcdkOpenCVView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	Initial();
	return 0;
}


void CAcdkOpenCVView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	Exit();
}

IplImage* CAcdkOpenCVView::T9_Mosaic2(IplImage *img, int div)
{
	if (div < 1) div = 1;

	resize_img = cvCreateImage(cvSize(img->width / div, img->height / div), 8, 3);
	mosaic_img2 = cvCreateImage(cvSize(img->width, img->height), 8, 3);

	cvResize(img, resize_img, CV_INTER_LINEAR);        // 줄였다가
	cvResize(resize_img, mosaic_img2, CV_INTER_NN);  // 키웠을 뿐..

	return mosaic_img2;
}
typedef struct stMap{

	unsigned char r;
	unsigned char g;
	unsigned char b;

} IMGMAP;

IMGMAP **Maps;

void Img2Aray(IplImage* img)
{
	//여기가 2차원 배열 만드는 곳이예요~ 동적으로 생성하는 것이요~
	Maps = new IMGMAP*[img->height];

	for (int i = 0; i < img->width; i++){
		Maps[i] = new IMGMAP[img->width];
	}

	for (int i = 0; i < img->height; i++){
		for (int j = 0; j < img->width; j++){

			Maps[i][j].r = img->imageData[i*img->widthStep + j*img->nChannels + 2];
			Maps[i][j].g = img->imageData[i*img->widthStep + j*img->nChannels + 1];
			Maps[i][j].b = img->imageData[i*img->widthStep + j*img->nChannels + 0];

		}
	}
}
IplImage* CAcdkOpenCVView::contour(IplImage* img)   //윤곽
{
	int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 },
		dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int mask[3][3] = { { -1, -1, -1 },
	{ -1, 8, -1 },
	{ -1, -1, -1 } };
	int p;
	Img2Aray(img);

	for (int i = 1; i < img->height - 1; i++){
		for (int j = 1; j < img->width - 1; j++){
			p = Maps[i][j].r;
			p *= mask[1][1];
			for (int k = 0; k<8; k++){
				p = p + ((unsigned char)Maps[i + di[k]][j + dj[k]].r * mask[1 + di[k]][1 + dj[k]]);
			}
			if (p > 255) p = 255;
			else if (p < 0) p = 0;
			img->imageData[i*img->widthStep + j*img->nChannels + 2] = (unsigned char)p;
			//여기를 주목	
			img->imageData[i*img->widthStep + j*img->nChannels + 1] = (unsigned char)p;
			img->imageData[i*img->widthStep + j*img->nChannels + 0] = (unsigned char)p;
		}
	}
	return img;
}

void CAcdkOpenCVView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 0:
		break;
	case 1:
		DrawCamPicture();
		break;

	}



	CView::OnTimer(nIDEvent);
}
void CAcdkOpenCVView::Initial()
{

	// 	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// 	_CrtSetBreakAlloc(143);

	capture = cvCaptureFromCAM(0); // 0번째 카메라 연결
	cvNamedWindow("T9-camera", 0); // 원본 영상을 띄울 윈도우, 0 은 고정된 윈도우 생성
	cvResizeWindow("T9-camera", 320, 240); // T9라는 윈도우를 320, 240으로 조정
	cvNamedWindow("T9-output", 0); // 결과 영상을 띄울 윈도우
	cvCreateTrackbar("T", "T9-camera", &threshold, 255, NULL); // "T9-camera" 윈도우에 bar 를 붙이고 threshold 값을 변화 시킴
	cvCreateTrackbar("MOSAIC", "T9-camera", &mosaicdiv, 255, NULL);
	SetTimer(1, 100, NULL);

}
void CAcdkOpenCVView::Exit()
{
	//cvReleaseImage(&image);
	cvReleaseImage(&resize_img);
	cvReleaseImage(&mosaic_img2);
	cvReleaseImage(&convert);
	cvReleaseImage(&gray);
	cvReleaseImage(&output);
	cvReleaseCapture(&capture);
	cvDestroyWindow("T9-output");
	cvDestroyWindow("T9-camera");
}
void CAcdkOpenCVView::DrawCamPicture()
{
	// 이름은 "T" 를 붙이고 0 부터 255 사이의 범위값을 가짐

	cvGrabFrame(capture);									//카메라로부터 한프레임을 잡음 
	image = cvRetrieveFrame(capture);						// 잡은 프레임을 image에 넣음
	IplImage* mosaic = T9_Mosaic2(image, mosaicdiv);


	convert = contour(image);
	cvShowImage("T9-contour", convert);
	cvShowImage("T9-camera", mosaic);
	if (image == NULL)
	{
		printf("No Capture Image. Check your camera.");
		//continue;
		return;
	}
	if (!output ){
		gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성
		output = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); // 흑백 이미지 생성

	}
	cvCvtColor(image, gray, CV_RGB2GRAY); // 컬러를 흑백으로 변환  CV_RGB2YCrCb CV_RGB2HLS CV_RGB2HSV CV_RGB2Lab CV_RGB2Luv
	//cvCvtColor(image, gray, CV_RGB2YCrCb ); // CV_RGB2GRAY CV_RGB2YCrCb CV_RGB2HLS CV_RGB2HSV CV_RGB2Lab CV_RGB2Luv

	int c = cvWaitKey(10); // 키보드로부터 키 값을 받습니다.

	if (c == 27 || c == 'q') // esc == 27
	{
		KillTimer(1);
		//break;
		OnDestroy();
		return;
	}

	// 영상의 각 픽셀(x,y) 값이 threshold 값의 초과는 255 로, 그 이하는 0 으로 변환
	cvThreshold(gray, output, threshold, 255, CV_THRESH_BINARY);
	output->origin = image->origin; // 방향이 뒤집어 진것을 바로 잡아줌

	cvShowImage("T9-output", output);

	cvReleaseImage(&resize_img);
	cvReleaseImage(&mosaic_img2);
	//cvReleaseImage(&mosaic);
}

void CAcdkOpenCVView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages
}
