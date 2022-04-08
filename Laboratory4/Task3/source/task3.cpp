
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>


using namespace cv;
using namespace std;


//CANNY FUNCTION
cv::Mat myCanny(cv::Mat& image, int threshold1, int threshold2, int kernelBlur, int kernelCanny);


int main(int argc, char** argv)
{

	//VARIABLES
	std::string path = "../../images/street_scene.png";
	Mat img;
	Mat edges;
	string windowNameStreetScene = "Street Scene";
	string windowNameCannyImage = "Canny Image";
	string windowNameDetectedLines = "Detected Lines";

	//SHOW IMAGE LOADED FROM DISK
	img = imread(path);
	namedWindow(windowNameStreetScene);
	imshow(windowNameStreetScene, img);
	waitKey(0);	
	destroyAllWindows();

	//COMPUTE CANNY IMAGE THAT IDENTIFIES ONLY STRIPES IN THE STREET
	edges = myCanny(img, 0, 700, 3, 3);
	imshow(windowNameCannyImage, edges);
	waitKey(0);
	destroyAllWindows();
	

	vector<Vec2f> lines; 
	HoughLines(edges, lines, 1, CV_PI / 180, 120, 0, 0); 
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(img, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
	}
	namedWindow(windowNameDetectedLines);
	imshow(windowNameDetectedLines, img);
	waitKey(0);
	
	return 0;
}

cv::Mat myCanny(cv::Mat& image, int threshold1, int threshold2, int kernelBlur, int kernelCanny)
{
	Mat gray;
	Mat edges;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	blur(gray, edges, Size(kernelBlur, kernelBlur));
	Canny(edges, edges, threshold1, threshold2, kernelCanny);
	return edges;

}
