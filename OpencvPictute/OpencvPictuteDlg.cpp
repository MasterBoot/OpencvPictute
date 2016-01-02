
// OpencvPictuteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpencvPictute.h"
#include "OpencvPictuteDlg.h"
#include "afxdialogex.h"
#include "cv.h" 
#include "highgui.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpencvPictuteDlg 对话框



COpencvPictuteDlg::COpencvPictuteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpencvPictuteDlg::IDD, pParent)
	, m_strFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpencvPictuteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FilePath, m_strFilePath);
}

BEGIN_MESSAGE_MAP(COpencvPictuteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &COpencvPictuteDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_BUTFILE, &COpencvPictuteDlg::OnBnClickedButfile)
END_MESSAGE_MAP()


// COpencvPictuteDlg 消息处理程序

BOOL COpencvPictuteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpencvPictuteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpencvPictuteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

const char* cascade_name =
"haarcascade_frontalface_alt.xml";

void COpencvPictuteDlg::OnBnClickedRun()
{
	//cascade_name = "haarcascade_frontalface_alt2.xml";
	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0);
	if (!cascade)
	{
		MessageBox(L"没有找到人脸训练器文件");
		return;
	}
	storage = cvCreateMemStorage(0);
	cvNamedWindow("人脸识别", 1);

	//const char* filename = m_strFilePath;
	IplImage* image = cvLoadImage((char*)(_bstr_t)m_strFilePath, 1);
	if (image)
	{
		detect_and_draw(image);
		cvWaitKey(0);
		cvReleaseImage(&image);
	}

	cvDestroyWindow("人脸识别");
	this->ShowWindow(SW_SHOW);
	return;

}
void COpencvPictuteDlg::detect_and_draw(IplImage* img)
{
	double scale = 0.7; //缩放因子，图片缩小检测提高检测速度
	static CvScalar colors[] = {
		{ 0, 0, 255 }, { 0, 128, 255 }, { 0, 255, 255 }, { 0, 255, 0 },
		{ 255, 128, 0 }, { 255, 255, 0 }, { 255, 0, 0 }, { 255, 0, 255 }
	};//Just some pretty colors to draw with

	//Image Preparation 
	// 
	IplImage* gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	IplImage* small_img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);
	cvResize(gray, small_img, CV_INTER_LINEAR);

	cvEqualizeHist(small_img, small_img); //直方图均衡

	//Detect objects if any 
	// 
	cvClearMemStorage(storage);
	double t = (double)cvGetTickCount();
	CvSeq* objects = cvHaarDetectObjects(small_img, cascade, storage, 1.1, 2, 0, cvSize(30, 30));
	t = (double)cvGetTickCount() - t;
	//printf("detection time = %gms\n", t / ((double)cvGetTickFrequency() * 1000));
	//Loop through found objects and draw boxes around them 
	for (int i = 0; i<(objects ? objects->total : 0); ++i)
	{
		CvRect* r = (CvRect*)cvGetSeqElem(objects, i);
		cvRectangle(img, cvPoint(r->x*scale, r->y*scale), cvPoint((r->x + r->width)*scale, (r->y + r->height)*scale), colors[i % 8]);
	}
	int i;
	CString str;
	for (i = 0; i < (objects ? objects->total : 0); i++)
	{
		CvRect* r = (CvRect*)cvGetSeqElem(objects, i);
		CvPoint center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale); //获取圆心X坐标
		center.y = cvRound((r->y + r->height*0.5)*scale);//获取圆心Y坐标 
		radius = cvRound((r->width + r->height)*0.25*scale); //获取半径
		//对img脸部进行圆圈标注。参数为：图像，圆心坐标，圆心半径，线条颜色，线条粗细负数表示填充元，线条类型，圆心坐标点和半径值的小数点位数
		cvCircle(img, center, radius, colors[i % 8], 1, 8, 0);
	}
	str.Format(L"一共检测到%d张人脸,耗时%g毫秒", i, t / ((double)cvGetTickFrequency() * 1000));
	cvShowImage("人脸识别", img); //显示结果窗口
	cvReleaseImage(&gray); //显示图像
	cvReleaseImage(&small_img);
	this->ShowWindow(SW_HIDE);
	MessageBox(str);


}


void COpencvPictuteDlg::OnBnClickedButfile()
{
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	LPCTSTR lpszFilter = L"JPEG文件(*.jpg)|*.jpg|BMP文件(*.bmp)|*.bmp|PNG文件(*.png)|*.png|所有文件(*.*)|*.*||";
	CFileDialog FileDlg(TRUE, NULL, NULL, dwFlags, lpszFilter, AfxGetMainWnd());
	//CString strPath , strText;
	if (FileDlg.DoModal() == IDOK)
	{
		m_strFilePath = FileDlg.GetPathName();
		UpdateData(false);

	}
}
