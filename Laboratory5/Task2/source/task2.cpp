#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "include/Helper.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	string pathImg1 = "../../images/street_scene.png";	
	string windowNameGray = "Gray";
	string windowNameRgb = "Rgb";
	int K = 3;

	Mat sourceImage = imread(pathImg1);
	if (sourceImage.empty())
	{
		cout << "CANNOT LOAD THE IMAGE";
		return -1;
	}
	
	Mat grayImage, rgbImage;
	cvtColor(sourceImage, grayImage, COLOR_BGR2GRAY);
	cvtColor(sourceImage, rgbImage, COLOR_BGR2RGB);
	
	Mat kMeansgray = Helper::myKMeans(grayImage, K);
	Mat kMeansRgb = Helper::myKMeans(rgbImage, K);

	cvtColor(kMeansRgb,kMeansRgb, COLOR_RGB2BGR); //go back to bgr
	imshow(windowNameGray, kMeansgray);
	imshow(windowNameRgb, kMeansRgb);
	waitKey();
	destroyAllWindows();

}
