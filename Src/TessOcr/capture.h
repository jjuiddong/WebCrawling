//
// 2018-10-16, jjuiddong
// Screen Capture 
//
// Reference Sample Code
//https://stackoverflow.com/questions/9524393/how-to-capture-part-of-the-screen-and-save-it-to-a-bmp
//
#pragma once


namespace tessocr
{

	class cCapture
	{
	public:
		cCapture();
		virtual ~cCapture();

		bool Capture(int x, int y, int w, int h, const char *fileName);
		void Clear();


	protected:
		bool SaveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);


	public:
	};

}
