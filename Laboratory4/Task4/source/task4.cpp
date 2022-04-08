#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	string path = "../../images/street_scene.png";
	string nameWindowStreeScene = "Street Scene";
	string nameWindowDetectedCircles = "Detected Circles";
	Mat gray;
	Mat img;

	img = imread(path);
	namedWindow(nameWindowStreeScene);
	imshow(nameWindowStreeScene, img);
	waitKey(0);
	destroyAllWindows();

	cvtColor(img, gray, COLOR_BGR2GRAY);
	medianBlur(gray, gray, 7);

	vector<Vec3f> circles;
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
		gray.rows / 64, // Change this value to detect circles with different distances to each other
		700, //Internal Canny Chreashold
		10, //Threashold for center detection
		8, 10 // Change the last two parameters  (min_radius & max_radius) to detect larger circles
	);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		circle(img, center, 1, Scalar(0, 100, 100), 3, LINE_AA);		
		int radius = c[2];
		circle(img, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
	}
	imshow(nameWindowDetectedCircles, img);
	waitKey();
	return 0;
}

