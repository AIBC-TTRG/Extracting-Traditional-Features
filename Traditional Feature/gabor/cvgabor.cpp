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
#include "cvgabor.h"


CvGabor::CvGabor()
{
}


CvGabor::~CvGabor()
{
}


/*!difference Parameters of inti

Parameters:
iMu        The orientation iMu*PI/8,
iNu         The scale,
dSigma         The sigma value of Gabor,
dPhi        The orientation in arc
dF        The spatial frequency

*/

CvGabor::CvGabor(int iMu, int iNu)
{
    double dSigma = 2*PI;
    F = sqrt(2.0);
    Init(iMu, iNu, dSigma, F);
}

CvGabor::CvGabor(int iMu, int iNu, double dSigma)
{ 
    F = sqrt(2.0);
    Init(iMu, iNu, dSigma, F);
}

CvGabor::CvGabor(int iMu, int iNu, double dSigma, double dF)
{
    Init(iMu, iNu, dSigma, dF);
}

CvGabor::CvGabor(double dPhi, int iNu)
{
    Sigma = 2*PI;
    F = sqrt(2.0);
    Init(dPhi, iNu, Sigma, F);
}

CvGabor::CvGabor(double dPhi, int iNu, double dSigma)
{
    F = sqrt(2.0);
    Init(dPhi, iNu, dSigma, F);
}

CvGabor::CvGabor(double dPhi, int iNu, double dSigma, double dF)
{
    Init(dPhi, iNu, dSigma,dF);
}

/*!
Parameters:
iMu     The orientations which is iMu*PI.8
iNu     The scale can be from -5 to infinit
dSigma     The Sigma value of gabor, Normally set to 2*PI
dF     The spatial frequence , normally is sqrt(2)

Initilize the.gabor with the orientation iMu, the scale iNu, the sigma dSigma, the frequency dF, it will call the function creat_kernel(); So a gabor is created.
*/
void CvGabor::Init(int iMu, int iNu, double dSigma, double dF)
{
    //Initilise the parameters
    bInitialised = false;
    bKernel = false;

    Sigma = dSigma;
    F = dF;

    Kmax = PI/2;

    //Absolute value of K
    K = Kmax / pow(F, (double)iNu); //K = Kmax / F^iNu
    Phi = PI*iMu/8;
    bInitialised = true;

    Width = mask_width();
    creat_kernel();
}

/*!
Parameters:
dPhi     The orientations
iNu     The scale can be from -5 to infinit
dSigma     The Sigma value of gabor, Normally set to 2*PI
dF     The spatial frequence , normally is sqrt(2)

Initilize the.gabor with the orientation dPhi, the scale iNu, the sigma dSigma, the frequency dF, it will call the function creat_kernel(); So a gabor is created.filename     The name of the image file
file_format     The format of the file,
*/
void CvGabor::Init(double dPhi, int iNu, double dSigma, double dF)//ok,is this!
{

    bInitialised = false;
    bKernel = false;
    Sigma = dSigma;
    F = dF;

    Kmax = PI/2;

    // Absolute value of K
    K = Kmax / pow(F, (double)iNu);
    Phi = dPhi;
    bInitialised = true;

    Width = mask_width();//return a odd of width
    creat_kernel();
}

/*!
Returns:
a boolean value, TRUE is created or FALSE is non-created.

Determine whether a gabor kernel is created.
*/

bool CvGabor::IsInit()
{
    return bInitialised;
}

bool CvGabor::IsKernelCreate()
{
    return bKernel;
}

/*!
Returns:
The long type show the width.

Return the width of mask (should be NxN) by the value of Sigma and iNu.
*/
int CvGabor::mask_width()
{
    int lWidth;
    if (IsInit() == false)
    {
        cerr << "Error: The Object has not been initilised in mask_width()!\n" << endl;
        return 0;
    }
    else
    {
        //determine the width of Mask
        double dModSigma = Sigma/K;
        
        int dWidth = cvRound(dModSigma*6 + 1);

        //test whether dWidth is an odd.
        if((dWidth % 2) == 0)
        {
            lWidth = dWidth + 1;
        }
        else
        {
            lWidth = dWidth;
        }
        return lWidth;
    }
}

/*!

Returns:
Pointer to long type width of mask.

*/
int CvGabor::get_mask_width()
{
    // printf("width:%f\n",Width);
    return Width;
}

/*!
\fn CvGabor::creat_kernel()
Create gabor kernel

Create 2 gabor kernels - REAL and IMAG, with an orientation and a scale 
*/
void CvGabor::creat_kernel()
{

    if (IsInit() == false)
    {
        cerr << "Error: The Object has not been initilised in creat_kernel()!" << endl;
    }
    else
    {
        Mat mReal(Width, Width, CV_32FC1);
        Mat mImag(Width, Width, CV_32FC1);

        /**************************** Gabor Function ****************************/ 
        int x, y;
        double dReal;
        double dImag;
        double dTemp1, dTemp2, dTemp3;

        for (int i = 0; i < Width; i++)
        {
            for (int j = 0; j < Width; j++)
            {
                x = i-(Width-1)/2;
                y = j-(Width-1)/2;
                dTemp1 = (pow(K,2)/pow(Sigma,2))*exp(-(pow((double)x,2)+pow((double)y,2))*pow(K,2)/(2*pow(Sigma,2)));
                dTemp2 = cos(K*cos(Phi)*x + K*sin(Phi)*y) - exp(-(pow(Sigma,2)/2));
                dTemp3 = sin(K*cos(Phi)*x + K*sin(Phi)*y);
                dReal = dTemp1*dTemp2;
                dImag = dTemp1*dTemp3;

                mReal.row(i).col(j) = dReal;
                mImag.row(i).col(j) = dImag;
            }
        }
        /**************************** Gabor Function ****************************/
        bKernel = true;

        mReal.copyTo(Real);
        mImag.copyTo(Imag);
    }
}


/*!
\fn CvGabor::get_image(int Type)
Get the speific type of image of Gabor

Parameters:
Type        The Type of gabor kernel, e.g. REAL, IMAG, MAG, PHASE   

Returns:
Pointer to image structure, or NULL on failure    

Return an Image (gandalf image class) with a specific Type   "REAL"    "IMAG" "MAG" "PHASE"  
*/
void CvGabor::get_image(int Type, Mat& image)
{
    if(IsKernelCreate() == false)
    { 
        cerr << "Error: the Gabor kernel has not been created in get_image()!" << endl;
        return ;
    }
    else
    {  
        Mat re(Width, Width, CV_32FC1);
        Mat im(Width, Width, CV_32FC1);
        Mat temp;

        switch(Type)
        {
        case 1:  //Real
            temp = Real.clone();
            // std::cout <<"temp:      " << temp << std::endl;
            normalize(temp, temp, 255.0, 0.0, NORM_MINMAX);
            break;
        case 2:  //Imag
            // std::cout <<"temp:      " << temp << std::endl;
            temp = Imag.clone();
            break; 
        case 3:  //Magnitude
            re = Real.clone();
            im = Imag.clone();
            
            pow(re, 2, re);     //re = re^2
            pow(im, 2, im);
            add(im, re, temp);  //temp = im + re
            pow(temp, 0.5, temp);
            break;
        case 4:  //Phase
            ///@todo
            break;
        }
        // std::cout <<"temp:      " << temp << std::endl;
        convertScaleAbs(temp, image, 1, 0);
        // std::cout <<"image:      " << temp << std::endl;
    }
}

/*!
\fn CvGabor::get_matrix(int Type)
Get a matrix by the type of kernel

Parameters:
Type        The type of kernel, e.g. REAL, IMAG, MAG, PHASE

Returns:
Pointer to matrix structure, or NULL on failure.

Return the gabor kernel.
*/
void CvGabor::get_matrix(int Type, Mat& matrix)
{
    if (!IsKernelCreate())
    {
        cerr << "Error: the gabor kernel has not been created!" << endl;
        return;
    }
    switch (Type)
    {
    case CV_GABOR_REAL:
        matrix = Real.clone();
        break;
    case CV_GABOR_IMAG:
        matrix = Imag.clone();
        break;
    case CV_GABOR_MAG:
        break;
    case CV_GABOR_PHASE:
        break;
    }
}

/*!
\fn CvGabor::conv_img_a(IplImage *src, IplImage *dst, int Type)
*/
void CvGabor::conv_img(Mat &src, Mat &dst, int Type)
{
    Mat mat = src.clone();

    Mat rmat(src.rows, src.cols, CV_32FC1);
    Mat imat(src.rows, src.cols, CV_32FC1);

    switch (Type)
    {
    case CV_GABOR_REAL:
        filter2D(mat, mat, 1, Real, Point((Width-1)/2, (Width-1)/2));
        // std::cout << "Width: " << Width << std::endl;
        break;

    case CV_GABOR_IMAG:
        filter2D(mat, mat, 1, Imag, Point( (Width-1)/2, (Width-1)/2));
        break;

    case CV_GABOR_MAG:
        /* Real Response */
        filter2D(mat, rmat, 1, Real, Point((Width-1)/2, (Width-1)/2));

        /* Imag Response */
        filter2D(mat, imat, 1, Imag, Point( (Width-1)/2, (Width-1)/2));

        /* Magnitude response is the square root of the sum of the square of real response and imaginary response */
        pow(rmat, 2, rmat);
        pow(imat, 2, imat);
        add(rmat, imat, mat);//mat = rmat + imat
        pow(mat, 0.5, mat);
        break;

    case CV_GABOR_PHASE:
        //@todo
        break;
    }

    // cvNormalize(mat, mat, 0, 255, CV_MINMAX, NULL);
    
    mat.copyTo(dst);
}



int main()
{
    
    double Sigma = PI/2; //don't know why
    double F = sqrt(2.0);
    CvGabor gabor(PI/4, 3, Sigma, F);
    
    Mat kernel(gabor.get_mask_width(), gabor.get_mask_width(), CV_8UC1);
    gabor.get_image(CV_GABOR_REAL, kernel); //kernel = gabor_real

    imshow("Kernel", kernel);
    cout << "kernel.rows:  " << kernel.rows << endl;
    cout << "kernel.clos:  " << kernel.cols << endl;

    // flags = -1 --> decode ;flags = 0 --> garyImg ;flags = 1 --> RGBimg
    Mat img = cv::imread("/home/aibc/Wen/yolo_uav/test2/data/t3/495.jpg", 0);
    if(img.empty())
    {
        fprintf(stderr,"can not load image\n");
        return -1;
    }

    // Mat labImg;
    // cv::cvtColor(img, labImg, CV_BGR2Lab);
    // Mat img = imread(labImg, CV_LOAD_IMAGE_GRAYSCALE );
    imshow("Original Image", img);
    
    Mat reimg(img.rows,img.cols, CV_32FC1);
    gabor.conv_img(img, reimg, CV_GABOR_REAL);
    std::cout<< "reimg:             "<< reimg.size << std::endl;
    imshow("After Image", reimg);
    cvWaitKey(0);
    return 0;

}
