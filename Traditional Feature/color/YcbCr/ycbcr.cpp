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


// https://blog.csdn.net/liulianfanjianshi/article/details/11723237 
int main()
{
    // IplImage* src = cvLoadImage("/home/aibc/wallpaper/desktopwallpaper/001.jpg",1);
    // Mat img = imread("/home/aibc/wallpaper/desktopwallpaper/001.jpg",1);
        
    float imgy,imgcb,imgcr;

    IplImage* img_rgb=cvLoadImage("/home/aibc/wallpaper/desktopwallpaper/001.jpg");
    IplImage* ycbcr=cvCreateImage(cvGetSize(img_rgb),8,3);      // 8 bit depth of image elements,3 channels
    IplImage* r_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* g_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* b_img_rgb=cvCreateImage(cvGetSize(img_rgb),8,1);
    IplImage* ycb=cvCreateImage(cvGetSize(img_rgb),8,3);  
    
    cvCvtColor(img_rgb,ycb,CV_BGR2YCrCb); 
    Mat img1 = cvarrToMat (ycb);
    std::cout << img1 << std::endl; 
    cvNamedWindow("ycbcr",WINDOW_NORMAL);
    cvResizeWindow("ycbcr", 480, 480);
    cvShowImage("ycbcr",ycb);
    cvWaitKey(0);
    return 0;
}
/*********************************Methods Two: Transformation formula of rgb->YcbCr******************************************/
/*
    cvSplit(img_rgb,r_img_rgb,g_img_rgb,b_img_rgb,0);//Three channels are divided into single channel

    IplImage* img_y = cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    IplImage* img_cb= cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    IplImage* img_cr= cvCreateImage (cvSize (img_rgb->width,img_rgb->height),IPL_DEPTH_8U, 1);
    // Transformation formula of rgb->YcbCr
    for(int y=0; y < img_rgb->height; y++) 
    {
        for(int x=0; x < img_rgb->width; x++)
        {    
            //transform

            imgy =0.257*cvGetReal2D(r_img_rgb,y,x)+0.504*cvGetReal2D(g_img_rgb,y,x)+0.098*cvGetReal2D(b_img_rgb,y,x)+16;    
            imgcb=-0.148*cvGetReal2D(r_img_rgb,y,x)-0.291*cvGetReal2D(g_img_rgb,y,x)+0.439*cvGetReal2D(b_img_rgb,y,x)+128;
            imgcr=0.439*cvGetReal2D(r_img_rgb,y,x)-0.368*cvGetReal2D(g_img_rgb,y,x)-0.071*cvGetReal2D(b_img_rgb,y,x)+128; 
            cvSetReal2D(img_y,y,x,imgy);
            cvSetReal2D(img_cb,y,x,imgcb);
            cvSetReal2D(img_cr,y,x,imgcr);
    
        }
    }
    
    cvMerge(img_y,img_cb,img_cr,NULL,ycbcr);//merge space of Ycbcr
    Mat img_ycbcr = cvarrToMat(ycbcr);
    Mat img = cvarrToMat(img_rgb);
    
    // Ycbcr -> RGB for display
    int imgr,imgg,imgb;
    IplImage* r_rgb=cvCreateImage(cvGetSize(ycbcr),8,1);
    IplImage* g_rgb=cvCreateImage(cvGetSize(ycbcr),8,1);
    IplImage* b_rgb=cvCreateImage(cvGetSize(ycbcr),8,1);
    IplImage* rgb=cvCreateImage(cvGetSize(ycbcr),8,3);
    // for display 
    for(int y=0;y < ycbcr->height;y++)
    {
        for(int x=0;x < ycbcr->width;x++)
        {
            imgr=1.164*(cvGetReal2D(img_y,y,x)-16)+1.596*(cvGetReal2D(img_cr,y,x)-128);
            imgg=1.164*(cvGetReal2D(img_y,y,x)-16)-0.392*(cvGetReal2D(img_cb,y,x)-128)-0.813*(cvGetReal2D(img_cr,y,x)-128);
            imgb=1.164*(cvGetReal2D(img_y,y,x)-16)+2.017*(cvGetReal2D(img_cb,y,x)-128);
            cvSetReal2D(r_rgb,y,x,imgr);
            cvSetReal2D(g_rgb,y,x,imgg);
            cvSetReal2D(b_rgb,y,x,imgb);
        }
    }
    cvMerge(r_rgb,g_rgb,b_rgb,NULL,rgb);        //merge space of rgb 
    cvNamedWindow("ycbcr",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ycb",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("y",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("cb",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("cr",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("rgb",CV_WINDOW_AUTOSIZE);
    cvShowImage("ycbcr",ycbcr);
    cvShowImage("y",img_y);
    cvShowImage("cb",img_cb);
    cvShowImage("cr",img_cr);
    cvShowImage("ycb",ycb);
    cvShowImage("rgb",rgb);
    cvWaitKey(0);
    cvDestroyWindow("ycbcr");
    cvDestroyWindow("cb");
    cvDestroyWindow("cr");
    cvDestroyWindow("ycb");
    cvDestroyWindow("rgb");
    cvDestroyWindow("y");
    cvReleaseImage(&ycbcr);
    cvReleaseImage(&img_y);
    cvReleaseImage(&img_cb);
    cvReleaseImage(&img_cr);
    cvReleaseImage(&ycb);
    cvReleaseImage(&rgb);
    */

