#include "include/RoadLinesDetector.h"



RoadLinesDetector::RoadLinesDetector(cv::Mat& image)
{
	this->image = image;
}

cv::Mat RoadLinesDetector::detectRoadLines(int threasholdWitheLines)
{
	cv::Mat finalImage, blurred, gradientX, absGradientX;

	cv::blur(image, blurred, cv::Size(KERNEL_BLUR,KERNEL_BLUR));
	blurred = getAboveThreashold(blurred, threasholdWitheLines);
	cv::Sobel(blurred, gradientX, CV_16S, ORDER_DERIVATIVE_X, 0, KERNEL_SOBEL, SCALE_FACTOR_SOBEL, DELTA_SOBEL, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(gradientX, absGradientX);
	finalImage = getAboveThreashold(absGradientX, threasholdWitheLines);
	return finalImage;
}

cv::Mat RoadLinesDetector::getAboveThreashold(cv::Mat& image, int threashold)
{
	cv::Mat ret(image.rows, image.cols, image.type());
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image.at<unsigned char>(i, j) >= threashold)
				ret.at<unsigned char>(i, j) = image.at<unsigned char>(i, j);

		}
	}
	return ret;
}
