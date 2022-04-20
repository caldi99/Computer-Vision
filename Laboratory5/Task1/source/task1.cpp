#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "include/Helper.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	char pathImg[200];
	string windowName = "Image";
	for (int i = 1; i <= 3; i++)
	{
		sprintf(pathImg, "../../images/Asphalt-%d.png", i);
		string path(pathImg);
	
		Mat sourceImage = imread(path);
		if (sourceImage.empty())
		{
			cout << "CANNOT LOAD THE IMAGE";
			return -1;
		}

		Mat finalImage = Helper::mySegmentation(sourceImage);
		imshow(windowName, finalImage);
		waitKey();
		destroyAllWindows();		
	}	
	return 0;
}

