// Screen Capture & Recognition

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "tesseract40d.lib")
#pragma comment(lib, "opencv_core340d.lib")
#pragma comment(lib, "opencv_imgproc340d.lib")
#pragma comment(lib, "opencv_imgcodecs340d.lib")

using namespace std;
using namespace cv;

int chrome_capture();
bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);
bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname);


int main()
{
	
	HWND hwnd = NULL;
	while (true)
	{
		hwnd = FindWindowEx(0, hwnd, L"Chrome_WidgetWin_1", NULL);
		if (!hwnd)
			break;
		if (!IsWindowVisible(hwnd))
			continue;

		RECT rc;
		GetWindowRect(hwnd, &rc);
		const int w = rc.right - rc.left;
		const int h = rc.bottom - rc.top;

		string imPath = "test.bmp";
		screenCapturePart(rc.left+10
			, rc.top + 485
			, 451
			, 90
			, imPath.c_str());


		string outText;

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

		break;
	}
}


//https://stackoverflow.com/questions/9524393/how-to-capture-part-of-the-screen-and-save-it-to-a-bmp
bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname)
{
	HDC hdcSource = GetDC(NULL);
	HDC hdcMemory = CreateCompatibleDC(hdcSource);

	int capX = GetDeviceCaps(hdcSource, HORZRES);
	int capY = GetDeviceCaps(hdcSource, VERTRES);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

	BitBlt(hdcMemory, 0, 0, w, h, hdcSource, x, y, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

	DeleteDC(hdcSource);
	DeleteDC(hdcMemory);

	HPALETTE hpal = NULL;
	if (saveBitmap(fname, hBitmap, hpal)) return true;
	return false;
}


//https://stackoverflow.com/questions/9524393/how-to-capture-part-of-the-screen-and-save-it-to-a-bmp
bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
	bool result = false;
	PICTDESC pd;

	pd.cbSizeofstruct = sizeof(PICTDESC);
	pd.picType = PICTYPE_BITMAP;
	pd.bmp.hbitmap = bmp;
	pd.bmp.hpal = pal;

	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
		reinterpret_cast<void**>(&picture));

	if (!SUCCEEDED(res))
		return false;

	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	HANDLE file = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (!SUCCEEDED(res) || !file)
	{
		stream->Release();
		picture->Release();
		return false;
	}

	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);
	LPVOID data = GlobalLock(mem);

	DWORD bytes_written;

	result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
	result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

	GlobalUnlock(mem);
	CloseHandle(file);

	stream->Release();
	picture->Release();

	return result;
}
