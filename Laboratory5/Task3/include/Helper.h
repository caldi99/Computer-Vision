#ifndef HELPER_H
#define HELPER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Helper
{
public:
	static cv::Mat mySegmentation(cv::Mat& inputImage);
	static cv::Mat myKMeans(cv::Mat& Input, int K);
	static cv::Mat thresholdTShirt(cv::Mat img, cv::Vec3b color, int accuracy);
};

#endif // !HELPER_H
