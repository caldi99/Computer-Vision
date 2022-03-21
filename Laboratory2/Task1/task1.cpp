#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

int main(int argc, char** argv)
{
	Mat img = imread("../../images/image.jpg");
	namedWindow("Example1",WINDOW_NORMAL);
	imshow("Example1", img);
	waitKey(0);

	cvtColor(img, img, COLOR_BGR2GRAY);
	imshow("Example1", img);
	imwrite("../../images/image_grayscale.jpg", img);
	waitKey(0);
	
	return 0;
}
