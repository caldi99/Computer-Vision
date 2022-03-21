
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;

int main(int argc, char** argv)
{
	Mat img = imread("../../images/image_grayscale.jpg", cv::IMREAD_GRAYSCALE); //force reading gray
	namedWindow("Example1", WINDOW_NORMAL);
	imshow("Example1", img);
	waitKey(0);

	int bins = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* rangePixels[] = { range };
	bool uniform = true, accumulate = false;

	Mat histogram;
	calcHist(&img, 1, 0, Mat(), histogram, 1, &bins, rangePixels, uniform, accumulate);
	


	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / bins);
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));


	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());


	for (int i = 1; i <= bins; i++)
	{
		line(histImage,
			Point(bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1))), 
			Point(bin_w*i, hist_h - cvRound(histogram.at<float>(i - 1))),
			Scalar(255, 0, 0), 1, 8, 0);
		
		line(histImage,
			Point(bin_w*(i-1), hist_h),
			Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Scalar(255, 0, 0), 1, 8, 0);
		
		line(histImage,
			Point(bin_w * i, hist_h),
			Point(bin_w * i, hist_h - cvRound(histogram.at<float>(i - 1))),
			Scalar(255, 0, 0), 1, 8, 0);

	}
	imshow("Example1", histImage);
	waitKey(0);
	
	return 0;
}
