
#include "stdafx.h"
#include "tess.h"

using namespace tessocr;


cTesseract::cTesseract()
	: m_tess(NULL)
{
}

cTesseract::~cTesseract()
{
	Clear();
}


bool cTesseract::Create()
{
	Clear();

	m_tess = new tesseract::TessBaseAPI();
	m_tess->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	m_tess->SetPageSegMode(tesseract::PSM_AUTO);

	return true;
}


// recognition text from image
// return utf8 text
//
// sample
//		cv::Mat im;
//		GetText(im.data, im.cols, im.rows, 3, im.step);
//
std::string cTesseract::GetText(const unsigned char *data, const int width, const int height
	, const int bytes_per_pixel, const int bytes_per_line)
{
	if (!m_tess)
		return {};

	m_tess->SetImage(data, width, height, bytes_per_pixel, bytes_per_line);
	return m_tess->GetUTF8Text();
}


void cTesseract::Clear()
{
	if (m_tess)
	{
		delete m_tess;
		m_tess = NULL;
	}
}
