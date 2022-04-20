#include "include/Helper.h"

cv::Mat Helper::myKMeans(cv::Mat& Input, int K)
{
	cv::Mat samples(Input.rows * Input.cols, Input.channels(), CV_32F);

	for (int y = 0; y < Input.rows; y++)
		for (int x = 0; x < Input.cols; x++)
			for (int z = 0; z < Input.channels(); z++)
				if (Input.channels() == 3) 				
					samples.at<float>(y + x * Input.rows, z) = Input.at<cv::Vec3b>(y, x)[z];				
				else 				
					samples.at<float>(y + x * Input.rows, z) = Input.at<uchar>(y, x);
				
	cv::Mat labels;
	cv::Mat centers;
	int attempts = 10;
	
	kmeans(samples, K, labels, cv::TermCriteria(cv::TermCriteria::COUNT, 10000, 0.0001), attempts, cv::KMEANS_PP_CENTERS, centers);
	cv::Mat new_image(Input.size(), Input.type());

	for (int y = 0; y < Input.rows; y++)
		for (int x = 0; x < Input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * Input.rows, 0);
			if (Input.channels() == 3)			
				for (int i = 0; i < Input.channels(); i++) 
					new_image.at<cv::Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);			
			else 
				new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);			
		}
	return new_image;
}

cv::Mat Helper::mySegmentation(cv::Mat& inputImage)
{
	cv::Mat srcImageGray;
	cv::Mat eroded;
	cv::Mat smoothedImage;
	cv::Mat thresholdImage;
	cv::Mat dilated;
	cv::cvtColor(inputImage, srcImageGray, cv::COLOR_BGR2GRAY);
	cv::erode(srcImageGray, eroded, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
	cv::GaussianBlur(eroded, smoothedImage, cv::Size(3, 3), 0, 0);
	cv::threshold(smoothedImage, thresholdImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	dilate(thresholdImage, dilated, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));
	return dilated;	
}

cv::Mat Helper::thresholdTShirt(cv::Mat img, cv::Vec3b color, int accuracy)
{
	cv::Mat ret(img.rows, img.cols, img.type());
	for (int r = 0; r < img.rows; r++)
	{
		for (int c = 0; c < img.cols; c++)
		{
			cv::Vec3b pixel = img.at<cv::Vec3b>(r, c);

			bool p1 = ((pixel[0] - color[0]) <= accuracy) && ((pixel[0] - color[0]) >= -accuracy);
			bool p2 = ((pixel[1] - color[1]) <= accuracy) && ((pixel[1] - color[1]) >= -accuracy);
			bool p3 = ((pixel[2] - color[2]) <= accuracy) && ((pixel[2] - color[2]) >= -accuracy);

			if (p1 && p2 && p3)
				ret.at<cv::Vec3b>(r, c) = pixel;
			else
				ret.at<cv::Vec3b>(r, c) = cv::Vec3b(0, 0, 0);
		}
	}
	return ret;
}
