//OPENCV
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//STL
#include <string>
#include <iostream>
#include <algorithm>

//MY LIB
#include "include/utils.h"

using namespace std;
using namespace cv;


int main(int argv, char** argc)
{
	string pathDirectory = "../images/checkerboard_images/";
	string extension = ".png";
	int numberImages = 57;

	MyUtility utility(pathDirectory, extension, numberImages);

	utility.findPatterns();
	utility.calibrateCamera();
	cout << "MEAN REPROJECTION ERROR : " << utility.comuputeMeanReprojectionError() << endl;
	vector<Mat> images = utility.undistortAndRectify(9);
	
	int rows = max(images.at(0).rows, images.at(1).rows);

	Mat img(rows, images.at(0).cols + 5 +  images.at(1).cols,CV_8UC1);

	//copy 1st img
	rows = min(max(images.at(0).rows, images.at(1).rows), images.at(0).rows);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < images.at(0).cols; j++)
		{
			img.at<unsigned char>(i, j) = images.at(0).at<unsigned char>(i, j);
		}
	}

	rows = max(images.at(0).rows, images.at(1).rows);
	//separation cols
	for (int i = 0; i < rows; i++)
	{
		for (int j = images.at(0).cols; j < images.at(0).cols + 5; j++)
		{
			img.at<unsigned char>(i, j) = 255;
		}
	}

	rows = min(max(images.at(0).rows, images.at(1).rows), images.at(1).rows);
	//copy 2nd image
	for (int i = 0; i < rows; i++)
	{
		for (int j = images.at(0).cols + 5; j < images.at(0).cols + 5 + images.at(1).cols; j++)
		{
			img.at<unsigned char>(i, j) = images.at(1).at<unsigned char>(i, j - (5 + images.at(0).cols));
		}
	}

	//On the left distorted on the rigth undistorted
	namedWindow("DISTORTED VS UNDISTORTED");
	imshow("DISTORTED VS UNDISTORTED", img);
	waitKey();

}
