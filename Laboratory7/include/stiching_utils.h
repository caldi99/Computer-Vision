#ifndef STICHING_UTILS
#define STICHING_UTILS

//OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//STL
#include <string>
#include <vector> 

class StichingUtils
{
public:
	StichingUtils(std::string pathDirectory,int numberFiles,std::string extension, double fov );	
	//void computePanorama(float ratio); //OLD
	cv::Mat stichImages(float ratio);
private:

	//method
	void readFiles(std::string pathDirectory, int numberFiles, std::string extension, double fov);
	std::vector< std::vector<cv::DMatch>> matchFeatures();
	std::vector< std::vector<cv::DMatch>> refineFeatures(std::vector< std::vector<cv::DMatch>>& features, float ratio);
	void computeMatricesTranslations(std::vector<std::vector<cv::DMatch>>& refinedFeatures, std::vector<int>& indices, std::vector<cv::Mat>& matrices);
	cv::Mat combineImages(std::vector<int>& indices, std::vector<cv::Mat>& matrices);
	void copyImage(cv::Mat& where_, cv::Mat& what, int offSet);
	int computeCol(cv::Mat& img,int threashold);

	//fields
	std::vector<cv::Mat> imagesProjected;
	std::vector<cv::Mat> descriptors;
	std::vector<std::vector<cv::KeyPoint>> keypoints;
};

#endif // STICHING_UTILS
