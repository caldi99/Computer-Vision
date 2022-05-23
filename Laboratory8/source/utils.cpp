

#include "include/utils.h"



MyUtility::MyUtility(std::string pathDirectory, std::string extension ,int numberImages)
{
	char* pathFile = new char[pathDirectory.size() + extension.size() + 14];

	for (int i = 1; i <= numberImages; i++)
	{
		sprintf(pathFile, pathDirectory.c_str());
		if (i < 10)
			sprintf(pathFile + pathDirectory.size(), "000%d_color", i);

		else
			sprintf(pathFile + pathDirectory.size(), "00%d_color", i);

		sprintf(pathFile + pathDirectory.size() + 10, extension.c_str());


		images.push_back(cv::imread(pathFile));
	}

	delete pathFile;
}

void MyUtility::findPatterns()
{	
	cv::Size chessBoardSize(INTERNALHEIGHTCORNERS, INTERNALWIDTHCORNERS);
	std::vector<cv::Point3f> points = computeObjectPoints();

	for (int i = 0; i < images.size(); i++)
	{
		std::vector<cv::Point2f> corners;
		cv::Mat imageGray;

		cv::cvtColor(images.at(i), imageGray, cv::COLOR_BGR2GRAY);

		if (bool success = findChessboardCorners(imageGray, chessBoardSize, corners))
		{
			//corners2??
			/*cornerSubPix(imageGray, corners, cv::Size(11, 11), cv::Size(-1, -1),
				cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));
			cv::drawChessboardCorners(imageGray, chessBoardSize, cv::Mat(corners), success);*/
			objectPoints.push_back(points);
			imagePoints.push_back(corners); //not corners2??
		}
	}

}

void MyUtility::calibrateCamera()
{
	cv::Mat cameraMatrix, distCoeff;
	std::vector<cv::Mat> rvecs, tvecs;	

	cv::calibrateCamera(objectPoints, imagePoints,
		cv::Size(INTERNALHEIGHTCORNERS, INTERNALWIDTHCORNERS), cameraMatrix, distCoeff, rvecs, tvecs);
	
	this->cameraMatrix = cameraMatrix;
	this->distCoeff = distCoeff;
	this->rvecs = rvecs;
	this->tvecs = tvecs;
}

float MyUtility::comuputeMeanReprojectionError()
{
	float meanError = 0;
	for (int i = 0; i < objectPoints.size(); i++)
	{
		std::vector<cv::Point2f> imagePoints2;
		cv::projectPoints(objectPoints.at(i), rvecs.at(i), tvecs.at(i), cameraMatrix, distCoeff, imagePoints2);
		meanError += cv::norm(imagePoints.at(i), imagePoints2, cv::NORM_L2) / imagePoints2.size();
	}
	return meanError / objectPoints.size();
}

std::vector<cv::Mat> MyUtility::undistortAndRectify(int indexImage)
{
	std::vector<cv::Mat> ret;

	cv::Mat img;
	cv::cvtColor(images.at(indexImage), img,cv::COLOR_BGR2GRAY);

	cv::Rect roi;
	cv::Mat newCameraMatrix = cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeff,
		cv::Size(img.cols, img.rows), 1, cv::Size(img.cols, img.rows), &roi);

	cv::Mat map1, map2;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeff, cv::Mat(),
		newCameraMatrix, cv::Size(img.cols, img.rows), 5, map1, map2);

	cv::Mat dest;
	cv::remap(img, dest, map1, map2, cv::INTER_LINEAR);

	cv::Mat resized(roi.height, roi.width, CV_8UC1);
	for (int i = roi.y; i < roi.y + roi.height; i++)
	{
		for (int j = roi.x; j < roi.x + roi.width; j++)
		{
			resized.at<unsigned char>(i - roi.y, j - roi.x) = dest.at<unsigned char>(i, j);
		}
	}

	ret.push_back(img);
	ret.push_back(resized);	
	return ret;
}


std::vector<cv::Point3f> MyUtility::computeObjectPoints()
{	
	std::vector<cv::Point3f> points;
	for (int i = 0; i < INTERNALWIDTHCORNERS; i++)	
		for (int j = 0; j < INTERNALHEIGHTCORNERS; j++)
			points.push_back(cv::Point3f(j, i, 0));
	return points;
}


