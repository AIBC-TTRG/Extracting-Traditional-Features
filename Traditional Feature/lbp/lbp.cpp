#include <opencv2/core/core.hpp>
//#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

const static int lbpBins = 14;
static float lbpSquare[lbpBins + 1][lbpBins + 1];

using namespace cv;
using namespace std;


void elbp(Mat& src, Mat &dst, int radius, int neighbors)
{

    for(int n=0; n<neighbors; n++)
    {
        
        float x = static_cast<float>(-radius * sin(2.0*CV_PI*n/static_cast<float>(neighbors)));
        float y = static_cast<float>(radius * cos(2.0*CV_PI*n/static_cast<float>(neighbors)));
        
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        
        float ty = y - fy;
        float tx = x - fx;
        
        float w1 = (1 - tx) * (1 - ty);
        float w2 =      tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 =      tx  *      ty;
        
        for(int i=radius; i < src.rows-radius;i++)
        {
            for(int j=radius;j < src.cols-radius;j++) 
            {
                
                float t = static_cast<float>(w1*src.at<uchar>(i+fy,j+fx) + w2*src.at<uchar>(i+fy,j+cx) + w3*src.at<uchar>(i+cy,j+fx) + w4*src.at<uchar>(i+cy,j+cx));
                
                dst.at<uchar>(i-radius,j-radius) += ((t > src.at<uchar>(i,j)) || (std::abs(t-src.at<uchar>(i,j)) < std::numeric_limits<float>::epsilon())) << n;
            }
        }
    }
    // dst.create(1, lbpBins * lbpBins, CV_32FC1);

	// for (int b = 0; b < lbpBins; b++)
	// {
	// 	for (int c = 0; c < lbpBins; c++)
	// 	{
	// 		*((float *)(dst.ptr(0)) + b * lbpBins + c) = lbpSquare[b][c];
	// 	}
	// }

	//cv::sqrt(dst, dst);
	//cv::normalize(dst, dst, 1, 0, NORM_L2, -1, noArray());
}

void elbp1(Mat& src, Mat &dst)
{
    for(int i=1; i < src.rows-1;i++)
    {
        for(int j=1;j < src.cols-1;j++) 
        {
            uchar tt = 0;
            int tt1 = 0;
            uchar u = src.at<uchar>(i,j);
            if(src.at<uchar>(i-1,j-1)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i-1,j)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i-1,j+1)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i,j+1)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i+1,j+1)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i+1,j)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i+1,j-1)>u) { tt += 1 <<tt1; } 
                tt1++;
            if(src.at<uchar>(i-1,j)>u) { tt += 1 <<tt1; } 
                tt1++;

            dst.at<uchar>(i-1,j-1) = tt;
        }
    }
}

int main()
{
    Mat img = cv::imread("/home/aibc/Wen/yolo_uav/test2/data/t3/1.jpg", 0);
    namedWindow("image");
    imshow("image", img);

    int radius, neighbors;
    radius = 1;
    neighbors = 8;

    Mat dst = Mat(img.rows-2*radius, img.cols-2*radius,CV_8UC1, Scalar(0));
    elbp1(img,dst);
    namedWindow("normal");
    imshow("normal", dst);
    //best
    Mat dst1 = Mat(img.rows-2*radius, img.cols-2*radius,CV_8UC1, Scalar(0));
    elbp(img,dst1,1,8);
    namedWindow("circle");
    imshow("circle", dst1);

    while(1)
     cv::waitKey(0);
}
