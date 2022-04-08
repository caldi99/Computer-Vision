#ifndef HANDLERTACKBARS_H
#define HANDLERTACKBARS_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string.h>

class HandlerTrackbars {

private:
	static int alpha;
	static int beta;
	static int kernelSize;
	static std::string nameWindow;
	static cv::Mat referenceImage;
	static cv::Mat myCanny(cv::Mat& image);

public:

	static void setReferenceImage(cv::Mat& image);
	static void setNameWindow(std::string name);

	static void onTrackbarAlpha(int value, void* pointer);
	static void onTrackbarBeta(int value, void* pointer);

};

#endif