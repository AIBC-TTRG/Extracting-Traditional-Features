#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
// #include <stdafx.h>
using namespace std;
using namespace cv;

#define PI 3.1415926535898

// schmid kernal
Mat getSchmidFilter(float tao, float sigma)
{
	float r = sigma/(4.0*tao);
	float sigma2 = sigma*sigma;
	// int half_filter_size = int(r+0.5);
	int half_filter_size = 10;
	int filter_size = 2 * half_filter_size + 1;
	printf("1\n");
	Mat schmid = Mat::zeros(filter_size,filter_size,CV_32F);
	
	float filter_sum = 0.0;
	for(int i = 0;i < filter_size; i++)
	{
		printf("2\n");
		float* s = schmid.ptr<float>(i);
		for(int j = 0; j < filter_size; j++)
		{
			printf("3\n");
			float x = i - half_filter_size;
			float y = j - half_filter_size;
			r = sqrt( x*x + y*y );
			float tmp = 2*PI * tao * r/sigma;
			float tmp2 = r*r / (2.0*sigma2);
			s[j] = cos(tmp)*exp(-tmp2);
			filter_sum += s[j];
		}
	}
	cout << filter_size << " " << filter_sum << endl;
	cout << schmid << endl;
	printf("4\n");
	if( abs(filter_sum - 0.0) < 1e-6)
	{
		return schmid;
	}

	// normalization 
	for(int i = 0; i < filter_size; i++)
	{
		// printf("complete schmid of normalization \n");
		float* s = schmid.ptr<float>(i);
		for(int j = 0; j < filter_size; j++)
		{
			s[j]/=filter_sum;
		}
	}
	
	return schmid;

}

int main()
{
	Mat img = imread("/home/aibc/wallpaper/desktopwallpaper/001.jpg",0);// garyImg
	if(img.empty())
    {
        fprintf(stderr,"can not load image\n");
        return -1;
    }
	//(tao, sigma) = (2,1), (4,1), (4,2), (6,1), (6,2), (6,3), (8,1), (8,2), (8,3), (10,1), (10,2), (10,3), (10,4),(12, 2)
    // float x = PI/2;
	// float y = 0.5;
	float x = 4.0;
	float y = 2.0;
    
	Mat schmid = getSchmidFilter(x,y);
	std::cout << schmid << std::endl;
	cvNamedWindow("kernel",WINDOW_NORMAL);
	resizeWindow("kernel", 480, 480);
	imshow("kernel",schmid);
	std::cout << "kernel: \n" << schmid.size << std::endl;
	
    Mat schmid_feature = img.clone();
	filter2D(img, schmid_feature, 1, schmid, Point((schmid.rows-1)/2, (schmid.cols-1)/2) );
    
    imshow("Original Image", img);
	imshow("schmid Image", schmid_feature);
	std::cout << "schmid: \n" << schmid_feature.size << std::endl;
	// std::cout << schmid_feature << std::endl;
	
    cvWaitKey(0);

	/*
	vector<Mat> imgs(14);				//set group of display picture  
	imgs[0] = getSchmidFilter(2, 1); 
	imgs[1] = getSchmidFilter(4, 1); 
	imgs[2] = getSchmidFilter(4, 2);
	imgs[3] = getSchmidFilter(6, 1); 

	imgs[4] = getSchmidFilter(6, 2);
	imgs[5] = getSchmidFilter(8, 1);
	imgs[6] = getSchmidFilter(8, 2);
	imgs[7] = getSchmidFilter(8, 3);

	imgs[8] = getSchmidFilter(10, 1);
	imgs[9] = getSchmidFilter(10, 2);
	imgs[10] = getSchmidFilter(10, 3);
	imgs[11] = getSchmidFilter(12, 2);

	Mat m ;
	hconcat(imgs[0], imgs[3], m); 
	hconcat(m, imgs[7], m); 
	hconcat(m, imgs[10], m); 
	hconcat(m, imgs[11], m); 

	namedWindow("test",0); 
	imshow("test", m);
	*/
	
    return 0;

}
