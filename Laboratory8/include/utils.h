#ifndef UTILS
#define UTILS

//STL
#include <string>
#include <vector>

//OPENCV
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
class MyUtility
{

public:
	MyUtility(std::string path, std::string extension, int numberImages);	

	void findPatterns();
	void calibrateCamera();
	float comuputeMeanReprojectionError();

	/* 1st image image of index indexImage, second one the one undistorted*/
	std::vector<cv::Mat> undistortAndRectify(int indexImage);

private:
	std::vector<cv::Point3f> computeObjectPoints();

	//Data after finding patterns
	std::vector<std::vector<cv::Point2f>> imagePoints;
	std::vector<std::vector<cv::Point3f> > objectPoints;

	//Data after calibration
	cv::Mat cameraMatrix;
	cv::Mat distCoeff;
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	//Chessboard constants
	static const int INTERNALWIDTHCORNERS = 6;
	static const int INTERNALHEIGHTCORNERS = 5;

	//Loaded Images
	std::vector<cv::Mat> images;	
};


#endif // UTILS
