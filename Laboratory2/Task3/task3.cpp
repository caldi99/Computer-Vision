
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;


int main(int argc, char** argv)
{
	Mat img = imread("../../images/image_grayscale.jpg", cv::IMREAD_GRAYSCALE); //force reading gray
	namedWindow("Example1", WINDOW_NORMAL);
	imshow("Example1", img);
	waitKey(0);

	Mat destinationImage;


	GaussianBlur(img, destinationImage, Size(7, 7), 0, 0);
	imshow("Example1", destinationImage);
	waitKey(0);

	medianBlur(img, destinationImage, 7);
	imshow("Example1", destinationImage);
	waitKey(0);
	
	return 0;
}
