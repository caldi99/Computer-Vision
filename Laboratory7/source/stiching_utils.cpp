//MY LIBRARIES
#include "include/stiching_utils.h"
#include "include/panoramic_utils.h"

//OPENCV
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

//STL
#include <string>

StichingUtils::StichingUtils(std::string pathDirectory, int numberFiles, std::string extension, double fov)
{
	//Read images, convert them and save
	readFiles(pathDirectory, numberFiles, extension, fov);

	//Extract and save keypoints and descriptos
	cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
	cv::Mat descriptors;
	std::vector<cv::KeyPoint> keypoints;

	for (cv::Mat img : imagesProjected)
	{
		detector->detectAndCompute(img, cv::noArray(), keypoints, descriptors);
		this->descriptors.push_back(descriptors);
		this->keypoints.push_back(keypoints);
	}
}

void StichingUtils::readFiles(std::string pathDirectory, int numberFiles, std::string extension, double fov)
{
	char* pathFile = new char[pathDirectory.size() + extension.size() + 3];

	cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
	cv::Mat descriptors;
	std::vector<cv::KeyPoint> keypoints;

	for (int i = 1; i <= numberFiles; i++)
	{
		sprintf(pathFile, pathDirectory.c_str());
		if (i < 10)
			sprintf(pathFile + pathDirectory.size(), "i0%d", i);

		else
			sprintf(pathFile + pathDirectory.size(), "i%d", i);


		sprintf(pathFile + pathDirectory.size() + 3, extension.c_str());

		cv::Mat img = cylindricalProj(cv::imread(pathFile), fov);
		imagesProjected.push_back(img);
	}
	delete pathFile;
}

cv::Mat StichingUtils::stichImages(float ratio)
{
	//Match features
	std::vector< std::vector<cv::DMatch>> matches = matchFeatures();

	//Refine descriptors
	std::vector< std::vector<cv::DMatch>> refinedFeatures = refineFeatures(matches,ratio);
	std::cout << refinedFeatures.size();


	//Compute matrixTranslations
	std::vector<int> indices;
	std::vector<cv::Mat> matrices;
	computeMatricesTranslations(refinedFeatures, indices, matrices);

	//Combine Images
	return combineImages(indices, matrices);
}

std::vector< std::vector<cv::DMatch>> StichingUtils::matchFeatures()
{
	std::vector< std::vector<cv::DMatch>> ret;

	cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);

	for (int i = 0; i < imagesProjected.size() - 1; i++)
	{
		cv::Mat descriptors1 = descriptors.at(i);
		cv::Mat descriptors2 = descriptors.at(i + 1);

		std::vector<cv::KeyPoint> keypoints1 = keypoints.at(i);
		std::vector<cv::KeyPoint> keypoints2 = keypoints.at(i + 1);

		std::vector<cv::DMatch> matches;
		matcher->match(descriptors1, descriptors2, matches);

		ret.push_back(matches);
	}
	return ret;
}

std::vector< std::vector<cv::DMatch>> StichingUtils::refineFeatures(std::vector< std::vector<cv::DMatch>>& features,float ratio)
{
	std::vector< std::vector<cv::DMatch>> ret;
	
	for (std::vector<cv::DMatch> featuresI : features)
	{
		sort(featuresI.begin(), featuresI.end());
		float threshold = featuresI.at(0).distance * ratio;
		
		std::vector<cv::DMatch> matchesRefined;
		
		for (int i = 0; i < featuresI.size(); i++)
		{
			if (featuresI.at(i).distance < threshold)
				matchesRefined.push_back(featuresI.at(i));
		}
		ret.push_back(matchesRefined);
	}
	return ret;
}

void StichingUtils::computeMatricesTranslations(std::vector< std::vector<cv::DMatch>>& refinedFeatures, std::vector<int>& indices, std::vector<cv::Mat>& matrices)
{
	
	for (int i = 0; i < refinedFeatures.size() - 1; i++)
	{
		//can only do operations for size > 4
		if (refinedFeatures.at(i).size() > 4)
		{
			std::vector<cv::DMatch> featuresI = refinedFeatures.at(i);

			std::vector<cv::Point2f> srcPoints;
			std::vector<cv::Point2f> dstPoints;
			
			for (int j = 0; j < featuresI.size(); j++)
			{
				srcPoints.push_back(keypoints.at(i).at(featuresI.at(j).queryIdx).pt);
				dstPoints.push_back(keypoints.at(i + 1).at(featuresI.at(j).trainIdx).pt);
			}
			
			matrices.push_back(cv::findHomography(srcPoints, dstPoints, cv::RANSAC));
			indices.push_back(i);
		}

	}
}

cv::Mat StichingUtils::combineImages(std::vector<int>& indices, std::vector<cv::Mat>& matrices)
{	
	cv::Mat img = imagesProjected.at(indices.at(0));
	int threshold = img.cols;
	cv::Mat finalImage;
	
	cv::warpPerspective(img, finalImage, matrices.at(0),
		cv::Size(img.cols * indices.size(), img.rows));

	int col = computeCol(finalImage, threshold);
	int offset = 0;

	copyImage(finalImage, imagesProjected.at(indices.at(0) + 1), offset);
	int totalSize = col + (imagesProjected.at(indices.at(0) + 1).cols - col);

	for (int i = 1; i < indices.size(); i++)
	{	
		//take next image
		img = imagesProjected.at(indices.at(i));

		//apply warpPerspective
		cv::Mat warp;
		cv::warpPerspective(img, warp, matrices.at(i),
			cv::Size(img.cols + imagesProjected.at(indices.at(i) + 1).cols, img.rows));

		//compute warp col
		int colWarp = computeCol(warp, threshold);

		

		//update offset += next image.cols - col
		offset += (imagesProjected.at(indices.at(i) + 1).cols - col);	

		/*
		TESTING
		std::cout <<"----------------" << std::endl;
		std::cout << "COL INDICIS : " << imagesProjected.at(indices.at(i) + 1).cols << std::endl;
		std::cout << "COL : " << col << std::endl;
		std::cout << "OFFSET : " << offset << std::endl;
		std::cout << "----------------" << std::endl;		
		*/
		copyImage(finalImage, imagesProjected.at(indices.at(i) + 1), offset);

		totalSize += ((colWarp + (imagesProjected.at(indices.at(i) + 1).cols - colWarp)) - offset);
		
		col = colWarp;
	}

	int colFinal = offset + imagesProjected.at(indices.size() - 1).cols;
	cv::Mat actualFinal(finalImage.rows, colFinal, finalImage.type());

	for (int r = 0; r < finalImage.rows; r++)
	{
		for (int c = 0; c < colFinal; c++)
		{
			actualFinal.at<unsigned char>(r, c) = finalImage.at<unsigned char>(r, c);
		}
	}
	return actualFinal;

}

int StichingUtils::computeCol(cv::Mat& img,int threashold)
{
	int col = img.cols-1;
	cv::Mat thrs = 255 - img;
	for (int x = col; x >= 0; x--)
	{
		if (thrs.at<unsigned char>(img.rows/2, x) != 255)
		{
			if (x < threashold)
			{				
				return x;				
			}
		}
	}		
	return col;
}

void StichingUtils::copyImage(cv::Mat& where_, cv::Mat& what, int offSet)
{
	for (int r = 0; r < what.rows; r++)
	{
		for (int c = 0; c < what.cols; c++)
		{
			where_.at<unsigned char>(r, c + offSet) = what.at<unsigned char>(r, c);
		}
	}
}

//OLD
/*void StichingUtils::computePanorama(float ratio)
{
	cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2);

	std::vector<cv::Mat> tranaslations;
	std::vector<int> indices;
	
	for (int i = 0; i < imagesProjected.size() - 1; i++)
	{
		cv::Mat descriptors1 = descriptors.at(i);
		cv::Mat descriptors2 = descriptors.at(i + 1);

		std::vector<cv::KeyPoint> keypoints1 = keypoints.at(i);
		std::vector<cv::KeyPoint> keypoints2 = keypoints.at(i+1);

		std::vector<cv::DMatch> matches;
		matcher->match(descriptors1, descriptors2, matches);
		
		sort(matches.begin(), matches.end());
		
		float threshold = matches.at(0).distance * ratio;

		std::vector<cv::DMatch> matchesRefined;
		for (int i = 0; i < matches.size(); i++)
		{
			if (matches.at(i).distance < threshold)			
				matchesRefined.push_back(matches.at(i));
		}
		
		//use refined matches with findHomography to find 	
		if (matchesRefined.size() > 4)
		{

			std::vector<cv::Point2f> srcPoints;
			std::vector<cv::Point2f> dstPoints;

			for (int i = 0; i < matchesRefined.size(); i++)
			{
				srcPoints.push_back(keypoints1.at(matchesRefined.at(i).queryIdx).pt);
				dstPoints.push_back(keypoints2.at(matchesRefined.at(i).trainIdx).pt);
			}

			cv::Mat mask = cv::findHomography(srcPoints,dstPoints,cv::RANSAC);

			tranaslations.push_back(mask);	
			indices.push_back(i);
		}
	}

	cv::Mat finalImage;
	cv::Mat img = imagesProjected.at(indices.at(0));
	cv::warpPerspective(img, finalImage, tranaslations.at(0),
		cv::Size(img.cols * indices.size(), img.rows));



	int col = computeCol(finalImage);
	int offset = 0;
	 
	copyImage(finalImage, imagesProjected.at(indices.at(0)+1), offset);
	int totalSize = col + (imagesProjected.at(indices.at(0) + 1).cols - col);
	


	for(int i=1; i < indices.size(); i++)
	{
		//take next image
		img = imagesProjected.at(indices.at(i));
		
		//apply warpPerspective

		cv::Mat warp;
		cv::warpPerspective(img, warp, tranaslations.at(i),
			cv::Size(img.cols + imagesProjected.at(indices.at(i) + 1).cols, img.rows));

		//computeCol warp
		int colWarp = computeCol(warp);

		//update offset += next image.cols - col
		offset += (imagesProjected.at(indices.at(i) + 1).cols - col);

		copyImage(finalImage, imagesProjected.at(indices.at(i) + 1), offset);

		totalSize += ((colWarp + (imagesProjected.at(indices.at(i) + 1).cols - colWarp)) - offset) ;
		//cv::imshow("Pippo", finalImage);
		//cv::waitKey();

		col = colWarp;
	

	}
	
	int colFinal = offset + imagesProjected.at(indices.size() - 1).cols;
	cv::Mat actualFinal(finalImage.rows, colFinal, finalImage.type());

	for (int r = 0; r < finalImage.rows; r++)
	{
		for (int c = 0; c < colFinal; c++)
		{
			actualFinal.at<unsigned char>(r, c) = finalImage.at<unsigned char>(r, c);
		}
	}

	cv::imwrite("file.jpg", actualFinal);
}*/
