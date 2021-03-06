//
// Template Match Test
//
#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
	#pragma comment(lib, "opencv_core340d.lib")
	#pragma comment(lib, "opencv_imgproc340d.lib")
	#pragma comment(lib, "opencv_imgcodecs340d.lib")
#else
	#pragma comment(lib, "opencv_core340.lib")
	#pragma comment(lib, "opencv_imgproc340.lib")
	#pragma comment(lib, "opencv_imgcodecs340.lib")
#endif

using namespace std;
using namespace cv;

int main()
{
	Mat label = cv::imread("label1.jpg");
	Mat bg = cv::imread("bg.jpg");
	Mat img_display;
	img_display = bg.clone();
	cvtColor(label, label, CV_RGB2GRAY);
	cvtColor(bg, bg, CV_RGB2GRAY);
	
	int match_method = CV_TM_SQDIFF;
	Mat result;
	matchTemplate(bg, label, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	
	// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	rectangle(img_display, matchLoc, Point(matchLoc.x + label.cols, matchLoc.y + label.rows)
		, Scalar(0,0,255), 2, 8, 0);
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	return 0;
}
