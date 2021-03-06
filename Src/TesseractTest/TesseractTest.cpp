// Tesseract Sample
// https://www.learnopencv.com/deep-learning-based-text-recognition-ocr-using-tesseract-and-opencv/

#include <iostream>
#include <string>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "tesseract40d.lib")
#pragma comment(lib, "opencv_core340d.lib")
#pragma comment(lib, "opencv_imgproc340d.lib")
#pragma comment(lib, "opencv_imgcodecs340d.lib")

using namespace std;
using namespace cv;

int main()
{
	string outText;
	string imPath = "image/text3.jpg";

	// Create Tesseract object
	tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

	// Initialize tesseract to use English (eng) and the LSTM OCR engine. 
	ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);

	// Set Page segmentation mode to PSM_AUTO (3)
	ocr->SetPageSegMode(tesseract::PSM_AUTO);

	// Open input image using OpenCV
	Mat im = cv::imread(imPath, IMREAD_COLOR);

	// Set image data
	ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);

	// Run Tesseract OCR on image
	outText = string(ocr->GetUTF8Text());

	// print recognized text
	cout << outText << endl; // Destroy used object and release memory ocr->End();

	delete ocr;
}
