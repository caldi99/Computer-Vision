#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "include/Helper.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	string pathImg1 = "../../images/robocup.jpg";
	string windowName = "Image";


	Mat sourceImage = imread(pathImg1);
	if (sourceImage.empty())
	{
		cout << "CANNOT LOAD THE IMAGE";
		return -1;
	}

	Vec3b orangeColor(50, 150, 200); 
	int accuracy = 70;

	Mat img = Helper::thresholdTShirt(sourceImage, orangeColor, accuracy);

	imshow(windowName, img);
	waitKey();
}
