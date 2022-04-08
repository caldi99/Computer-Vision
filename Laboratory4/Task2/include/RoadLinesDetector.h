#ifndef ROADLINESDETECTOR_H
#define ROADLINESDETECTOR_H


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


//Image to provide must be already GRAYSCALE
class RoadLinesDetector
{
public:
	RoadLinesDetector(cv::Mat& image);
	cv::Mat detectRoadLines(int threasholdWitheLines);
private:	
	cv::Mat getAboveThreashold(cv::Mat& image, int threashold);
		
	cv::Mat image;	
	
	const int KERNEL_BLUR = 3;
	const int ORDER_DERIVATIVE_X = 1;
	const int KERNEL_SOBEL = 3;
	const int SCALE_FACTOR_SOBEL = 3;
	const int DELTA_SOBEL = 3;
};


#endif

