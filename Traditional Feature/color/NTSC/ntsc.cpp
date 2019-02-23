#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace cv;


//adjust parameters
int main()
{
    float imgY,imgI,imgQ;
    IplImage* img_rgb=cvLoadImage("/home/aibc/wallpaper/desktopwallpaper/001.jpg");//25,9,8,18,9,16,25,17,24,33,19,17,29,15,17,28]
    
    IplImage* ycbcr=cvCreateImage(cvGetSize(img_rgb),8,3);
    IplImage* r_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* g_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* b_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* yiq=cvCreateImage(cvGetSize(img_rgb),8,3);  
    

    cvSplit(img_rgb,r_img_rgb,g_img_rgb,b_img_rgb,0);

    IplImage* img_Y = cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    IplImage* img_I = cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    IplImage* img_Q = cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    
    for(int y=0;y<img_rgb->height;y++) 
    {
        for(int x=0;x<img_rgb->width;x++)
        {    

            imgY =0.299*cvGetReal2D(r_img_rgb,y,x)+0.587*cvGetReal2D(g_img_rgb,y,x)+0.114*cvGetReal2D(b_img_rgb,y,x);    
            imgI =0.596*cvGetReal2D(r_img_rgb,y,x)-0.274*cvGetReal2D(g_img_rgb,y,x)-0.322*cvGetReal2D(b_img_rgb,y,x);
            imgQ =0.211*cvGetReal2D(r_img_rgb,y,x)-0.523*cvGetReal2D(g_img_rgb,y,x)+0.312*cvGetReal2D(b_img_rgb,y,x); 
            cvSetReal2D(img_Y,y,x,imgY);
            cvSetReal2D(img_I,y,x,imgI);
            cvSetReal2D(img_Q,y,x,imgQ);
    
        }
    }

    cvMerge(img_Y,img_I,img_Q,NULL,yiq);

    cvNamedWindow("yiq",CV_WINDOW_AUTOSIZE);
    cvShowImage("yiq",yiq);
    Mat ntsc = cvarrToMat(yiq);//size = org_img
    std::cout << ntsc.size << std::endl;
    
    /*
    int imgr,imgg,imgb;
    IplImage* r_rgb=cvCreateImage(cvGetSize(yiq),8,1);
    IplImage* g_rgb=cvCreateImage(cvGetSize(yiq),8,1);
    IplImage* b_rgb=cvCreateImage(cvGetSize(yiq),8,1);
    IplImage* rgb=cvCreateImage(cvGetSize(yiq),8,3);
    for(int y=0;y < yiq->height;y++)
    {
        for(int x=0;x < yiq->width;x++)
        {
            imgr=1.0*cvGetReal2D(img_Y,y,x)+0.956*cvGetReal2D(img_I,y,x)+0.619*cvGetReal2D(img_Q,y,x);
            imgg=1.0*cvGetReal2D(img_Y,y,x)-0.272*cvGetReal2D(img_I,y,x)-0.647*cvGetReal2D(img_Q,y,x);
            imgb=1.0*cvGetReal2D(img_Y,y,x)-1.106*cvGetReal2D(img_I,y,x)+1.703*cvGetReal2D(img_Q,y,x);
            cvSetReal2D(r_rgb,y,x,imgr);
            cvSetReal2D(g_rgb,y,x,imgg);
            cvSetReal2D( b_rgb,y,x,imgb);
        }
    }
    cvMerge(r_rgb,g_rgb,b_rgb,NULL,rgb);//22,  10,   7,  14,  15,  14,  16,  23,  22,  24,  21,  16,  25,  19,  16,  23]

    cvNamedWindow("yiq",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Y",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("I",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Q",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("rgb",CV_WINDOW_AUTOSIZE);
    
    cvShowImage("Y",img_Y);
    cvShowImage("I",img_I);
    cvShowImage("Q",img_Q);
    cvShowImage("yiq",yiq);
    cvShowImage("rgb",rgb);
    // Mat ntc = cvarrToMat(rgb);
    // std::cout << ntc << std::endl;
    cvWaitKey();

    cvDestroyWindow("Y");
    cvDestroyWindow("I");
    cvDestroyWindow("Q");
    cvDestroyWindow("rgb");

    cvReleaseImage(&ycbcr);
    cvReleaseImage(&img_Y);
    cvReleaseImage(&img_I);
    cvReleaseImage(&img_Q);
    cvReleaseImage(&rgb);
    */

    return 0;
}