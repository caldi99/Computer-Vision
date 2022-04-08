#include "include/HandlerTrackbars.h"


int HandlerTrackbars::alpha = 0;
int HandlerTrackbars::beta = 0;
int HandlerTrackbars::kernelSize = 3;
std::string HandlerTrackbars::nameWindow = "";
cv::Mat HandlerTrackbars::referenceImage = cv::Mat();

void HandlerTrackbars::onTrackbarAlpha(int value, void* pointer)
{
	alpha = value;
	cv::Mat imageToshow = myCanny(referenceImage);
	cv::imshow(nameWindow, imageToshow);

}

void HandlerTrackbars::onTrackbarBeta(int value, void* pointer)
{
	beta = value;
	cv::Mat imageToshow = myCanny(referenceImage);
	cv::imshow(nameWindow, imageToshow);
}

cv::Mat HandlerTrackbars::myCanny(cv::Mat& image)
{
	cv::Mat gray;
	cv::Mat edges;
	cv::Mat ret;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::blur(gray, edges, cv::Size(kernelSize, kernelSize));
	cv::Canny(edges, ret, alpha, beta, kernelSize);	
	return ret;
}

void HandlerTrackbars::setReferenceImage(cv::Mat& image)
{
	referenceImage = image;
}

void HandlerTrackbars::setNameWindow(std::string name)
{
	nameWindow = name;
}
