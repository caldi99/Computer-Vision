
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string.h>

#include "include/HandlerTrackbars.h"


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	//variables
	string pathImg = "../../images/street_scene.png"; 
	string streetSceneWindow = "Street Scene";
	string cannyImageWindow = "Canny Image";
	int maxValueBarAlpha = 1000;
	int maxValueBarBeta = 1000;
	Mat cannyImage;
	int alpha = 400;
	int beta = 200;
	int valueTrackbar = 0;
	
	

	//show loaded image standard one
	Mat img = imread(pathImg);
	namedWindow(streetSceneWindow);
	imshow(streetSceneWindow, img);
	waitKey(0);
	
		

	//show the canny image with alpha = 400 and beta = 200
	Canny(img, cannyImage, alpha, beta);
	namedWindow(cannyImageWindow);
	imshow(cannyImageWindow, cannyImage);
	waitKey(0);
	//destroyAllWindows();
	
	
	
	//Canny Image with trackbars
	namedWindow(cannyImageWindow);
	HandlerTrackbars::setNameWindow(cannyImageWindow);
	HandlerTrackbars::setReferenceImage(img);
	createTrackbar("Alpha: ", cannyImageWindow, &valueTrackbar, maxValueBarAlpha, HandlerTrackbars::onTrackbarAlpha);
	createTrackbar("Beta: ", cannyImageWindow, &valueTrackbar, maxValueBarBeta, HandlerTrackbars::onTrackbarBeta);
	imshow(cannyImageWindow, img);
	waitKey(0);
	destroyAllWindows();
	
	
	
	
	return 0;
}



