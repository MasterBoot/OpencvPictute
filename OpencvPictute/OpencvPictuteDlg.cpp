
// OpencvPictuteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpencvPictute.h"
#include "OpencvPictuteDlg.h"
#include "afxdialogex.h"
#include "cv.h" 
#include "highgui.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <math.h> 
#include <float.h> 
#include <limits.h> 
#include <time.h> 
#include <ctype.h>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpencvPictuteDlg �Ի���




COpencvPictuteDlg::COpencvPictuteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpencvPictuteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpencvPictuteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpencvPictuteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COpencvPictuteDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// COpencvPictuteDlg ��Ϣ�������

BOOL COpencvPictuteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpencvPictuteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpencvPictuteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );
void DetectAndMark();
const char *pcascadeName = "D:\\Opencv3.0\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
const char *pImageName = "face.jpg";

void COpencvPictuteDlg::OnBnClickedButton1()
{
	
	/*cascade = (CvHaarClassifierCascade*)cvLoad(pcascadeName, 0, 0, 0);
    if( !cascade ) 
    { 
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" ); 
       // return -1; 
    }*/
	//CvHaarClassifierCascade *pHaarClassCascade;
	//pHaarClassCascade = (CvHaarClassifierCascade*)cvLoad(pcascadeName);

   // storage = cvCreateMemStorage(0); 
   // cvNamedWindow( "����", 1 ); 
     
    //const char* filename = "face.jpg"; 
	//cvSize(img->width, img->height)
	/*IplImage* image = cvLoadImage(pImageName, 1);
    if( image ) 
    { 
        detect_and_draw( image ); 
        cvWaitKey(0); 
        cvReleaseImage( &image );   
    }

    cvDestroyWindow("����"); */

	DetectAndMark();
}

void detect_and_draw(IplImage* img ) 
{ 
    double scale=0.8; //�������ӣ�ͼƬ��С�����߼���ٶ�
	/*face.jpg����75������*/
    static CvScalar colors[] = { 
        (0,0,255),(0,128,255),(0,255,255),(0,255,0), 
        (255,128,0),(255,255,0),(255,0,0),(255,0,255) 
    };//Just some pretty colors to draw with

    //Image Preparation 
    // 
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
    IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
    cvCvtColor(img,gray, CV_BGR2GRAY); 
    cvResize(gray, small_img, CV_INTER_LINEAR);

    cvEqualizeHist(small_img,small_img); //ֱ��ͼ����

    //Detect objects if any 
    // 
    cvClearMemStorage(storage); 
    double t = (double)cvGetTickCount(); 
    CvSeq* objects = cvHaarDetectObjects(small_img, cascade,storage,1.1,2,0,cvSize(30,30));

    t = (double)cvGetTickCount() - t; 
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000) );

    //Loop through found objects and draw boxes around them 
	int faceCount;
	CString strCount;
    for(int i=0;i<(objects? objects->total:0);++i) 
    { 
        CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 
        cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]); 
    } 
    for( int i = 0; i < (objects? objects->total : 0); i++ ) //iΪ��⵽����������
    { 
        CvRect* r = (CvRect*)cvGetSeqElem( objects, i ); 
        CvPoint center; 
        int radius; 
        center.x = cvRound((r->x + r->width*0.5)*scale); //��ȡԲ��X����
        center.y = cvRound((r->y + r->height*0.5)*scale);//��ȡԲ��Y���� 
        radius = cvRound((r->width + r->height)*0.25*scale); //��ȡ�뾶
 //��img��������ԲȦ��ע������Ϊ��ͼ��Բ�����꣬Բ�İ뾶��������ɫ��������ϸ������ʾ���Ԫ���������ͣ�Բ�������Ͱ뾶ֵ��С����λ��
        cvCircle( img, center, radius, colors[i%8], 1, 8, 0 );
		faceCount = i;
    }
	strCount.Format(L"�Ѿ���⵽����������Ϊ��%d��", faceCount);
	//MessageBox(strCount);
    cvShowImage( "����", img ); //��ʾ�������
    cvReleaseImage(&gray); //��ʾͼ��
    cvReleaseImage(&small_img); 
}

void DetectAndMark()
{
	// load the Haar classifier    
	CvHaarClassifierCascade *pHaarClassCascade;
	pHaarClassCascade = (CvHaarClassifierCascade*)cvLoad(pcascadeName);

	//load the test image    
	IplImage *pSrcImage = cvLoadImage(pImageName, CV_LOAD_IMAGE_UNCHANGED);
	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	if (pSrcImage == NULL || pGrayImage == NULL)
	{
		printf("can't load image!\n");
		return;
	}
	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

	if (pHaarClassCascade != NULL && pSrcImage != NULL && pGrayImage != NULL)
	{
		const static CvScalar colors[] =
		{
			CV_RGB(0, 0, 255),
			CV_RGB(0, 128, 255),
			CV_RGB(0, 255, 255),
			CV_RGB(0, 255, 0),
			CV_RGB(255, 128, 0),
			CV_RGB(255, 255, 0),
			CV_RGB(255, 0, 0),
			CV_RGB(255, 0, 255)
		};

		CvMemStorage *pcvMemStorage = cvCreateMemStorage(0);
		cvClearMemStorage(pcvMemStorage);

		//detect the face    
		int TimeStart, TimeEnd;
		TimeStart = GetTickCount();
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarClassCascade, pcvMemStorage);
		TimeEnd = GetTickCount();

		printf("the number of faces: %d\nSpending Time: %d ms\n", pcvSeqFaces->total, TimeEnd - TimeStart);

		//mark the face     
		for (int i = 0; i <pcvSeqFaces->total; i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width * 0.5));
			center.y = cvRound((r->y + r->height * 0.5));
			radius = cvRound((r->width + r->height) * 0.25);
			cvCircle(pSrcImage, center, radius, colors[i % 8], 2);
		}
		cvReleaseMemStorage(&pcvMemStorage);
	}

	const char *pstrWindowsTitle = "FaceDetect Demo";
	cvNamedWindow(pstrWindowsTitle, CV_WINDOW_AUTOSIZE);
	cvShowImage(pstrWindowsTitle, pSrcImage);

	cvWaitKey(0);

	cvDestroyWindow(pstrWindowsTitle);
	cvReleaseImage(&pSrcImage);
	cvReleaseImage(&pGrayImage);
}
