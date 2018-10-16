//
// 2018-10-16, jjuiddong
// Tesseract OCR
//
#pragma once


namespace tessocr
{

	class cTesseract
	{
	public:
		cTesseract();
		virtual ~cTesseract();

		bool Create();
		std::string GetText(const unsigned char *data, const int width, const int height
			, const int bytes_per_pixel, const int bytes_per_line);
		void Clear();


	public:
		tesseract::TessBaseAPI *m_tess;
	};

}
