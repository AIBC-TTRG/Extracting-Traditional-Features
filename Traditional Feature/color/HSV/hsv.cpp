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

const static int hsvBins = 480;

#define cvQueryHistValue_2D( hist, idx0, idx1 )   cvGetReal2D( (hist)->bins, (idx0), (idx1) )

//  https://blog.csdn.net/shenmedoubu/article/details/24520717
int main( int argc, char** argv )
{
	IplImage* src = cvLoadImage("/home/aibc/wallpaper/desktopwallpaper/001.jpg",1);
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	
	int h_bins = 16, s_bins = 8;
	int hist_size[] = {h_bins, s_bins};
	
	float h_ranges[] = { 0, 180 };
	
    float s_ranges[] = { 0, 255 };
	float *ranges[] = { h_ranges, s_ranges }; 

	cvCvtColor( src, hsv, CV_BGR2HSV );

	cvSplit( hsv, h_plane, s_plane, v_plane, 0 );// cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 ); // extracts particular [color] plane
	
	CvHistogram* hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	
	cvCalcHist( planes, hist, 0, 0 );
	
	float max_value;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	// printf("max:%f\n",max_value);
	int height = 240;
	int width = (h_bins*s_bins*6);
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );//clears hist_img

	IplImage* hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage* rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			// Get the number of statistics in the histogram and calculate the height displayed in the image.
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			// printf("bin_val:%f\n",bin_val);
			int intensity = cvRound(bin_val*height/max_value);

			// Get the color represented by the current histogram and convert it to RGB for display
			cvSet2D(hsv_color, 0, 0, cvScalar(h*180.f / h_bins, s*255.f/s_bins, 255, 0));
			
			cvCvtColor(hsv_color, rgb_color, CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color, 0, 0);
			cvRectangle( hist_img, cvPoint(i*bin_w, height),
			cvPoint((i+1)*bin_w,height - intensity), color, -1, 8, 0 );
		}
	}
	cvNamedWindow( "Source", WINDOW_NORMAL);
	cvResizeWindow("Source",480, 480);
	cvShowImage( "Source", src );
	cvNamedWindow( "H-S Histogram", 1 );
	cvShowImage( "H-S Histogram", hist_img );
	cv::Mat hsvMat = cv::cvarrToMat(hist_img);//240 x 768
	// std::cout << hsvMat.size << std::endl;
	// std::cout << hsvMat << std::endl;
	
	// std::cout << hist << std::endl;
	
	cvWaitKey(0);
	
}
