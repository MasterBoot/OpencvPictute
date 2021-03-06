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
#ifdef _EiC 
#define WIN32 
#endif

static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

const char* cascade_name = 
"haarcascade_frontalface_alt.xml"; 
/*    "haarcascade_profileface.xml";*/

int main( int argc, char** argv ) 
{ 
    cascade_name = "haarcascade_frontalface_alt2.xml"; 
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); 
    if( !cascade ) 
    { 
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" ); 
        return -1; 
    } 
    storage = cvCreateMemStorage(0); 
    cvNamedWindow( "窗口", 1 ); 
     
    const char* filename = "face.jpg"; 
	//cvSize(img->width, img->height)
    IplImage* image = cvLoadImage( filename, 1 );
    if( image ) 
    { 
        detect_and_draw( image ); 
        cvWaitKey(0); 
        cvReleaseImage( &image );   
    }

    cvDestroyWindow("窗口"); 
  
    return 0; 
}

void detect_and_draw(IplImage* img ) 
{ 
    double scale=0.8; //缩放因子，图片缩小检测提高检测速度
	/*face.jpg共有75张人脸*/
    static CvScalar colors[] = { 
        {0,0,255},{0,128,255},{0,255,255},{0,255,0}, 
        {255,128,0},{255,255,0},{255,0,0},{255,0,255} 
    };//Just some pretty colors to draw with

    //Image Preparation 
    // 
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
    IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
    cvCvtColor(img,gray, CV_BGR2GRAY); 
    cvResize(gray, small_img, CV_INTER_LINEAR);

    cvEqualizeHist(small_img,small_img); //直方图均衡

    //Detect objects if any 
    // 
    cvClearMemStorage(storage); 
    double t = (double)cvGetTickCount(); 
    CvSeq* objects = cvHaarDetectObjects(small_img, cascade,storage,1.1,2,0,cvSize(30,30));

    t = (double)cvGetTickCount() - t; 
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000) );

    //Loop through found objects and draw boxes around them 
	int faceCount;
    for(int i=0;i<(objects? objects->total:0);++i) 
    { 
        CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 
        cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]); 
    } 
    for( int i = 0; i < (objects? objects->total : 0); i++ ) //i为检测到人脸的数量
    { 
        CvRect* r = (CvRect*)cvGetSeqElem( objects, i ); 
        CvPoint center; 
        int radius; 
        center.x = cvRound((r->x + r->width*0.5)*scale); //获取圆心X坐标
        center.y = cvRound((r->y + r->height*0.5)*scale);//获取圆心Y坐标 
        radius = cvRound((r->width + r->height)*0.25*scale); //获取半径
 //对img脸部进行圆圈标注。参数为：图像，圆心坐标，圆心半径，线条颜色，线条粗细负数表示填充元，线条类型，圆心坐标点和半径值的小数点位数
        cvCircle( img, center, radius, colors[i%8], 1, 8, 0 );
		faceCount = i;
    }

	printf("已经检测到的人脸个数为：%d个\n", faceCount);
    cvShowImage( "窗口", img ); //显示结果窗口
    cvReleaseImage(&gray); //显示图像
    cvReleaseImage(&small_img); 
}