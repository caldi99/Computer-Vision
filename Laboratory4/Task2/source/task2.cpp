#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "include/RoadLinesDetector.h"
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	string path = "../../images/street_scene.png";
	string windowName = "Street Scene";

	Mat img = imread(path);
	namedWindow(windowName);
	imshow(windowName, img);
	waitKey(0);

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	RoadLinesDetector detector(gray);
	Mat ret = detector.detectRoadLines(225);
	imshow(windowName, ret);
	waitKey(0);
	return 0;
}
