//MY LIBRARY
#include "include/stiching_utils.h"

//STL
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{
	//For time reasons I have not implemented how to read the from command line the parameters
	string pathDirectory = "../images/data/";
	int numberImages = 12;
	string extension = ".bmp";
	int FoV = 33;

	StichingUtils manager = StichingUtils(pathDirectory,numberImages,extension,FoV);
	
	imshow("IMAGE",manager.stichImages(10));
	waitKey();

	return 0;
}
