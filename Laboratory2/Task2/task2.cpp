#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filters.h"
#include <iostream>
using namespace cv;


int main(int argc, char** argv)
{
	Mat img = imread("../../images/image_grayscale.jpg", cv::IMREAD_GRAYSCALE); //force reading gray
	namedWindow("Example1", WINDOW_NORMAL);
	imshow("Example1", img);
	waitKey(0);

	Mat imgret = minFilter(img, 25); 
	imshow("Example1", imgret);
	waitKey(0);
	
	return 0;
}
