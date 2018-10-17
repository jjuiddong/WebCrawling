//
// 2018-10-16, jjuiddong
// Tesseract OCR library
//
#pragma once

#include <windows.h>
#include <tesseract/baseapi.h>
#include <string>
#include <vector>

using std::string;
using std::vector;


#include "tess.h"
#include "capture.h"
#include "keybot.h"
#include "config.h"
#include "utility.h"


#ifdef _DEBUG
	#pragma comment(lib, "tesseract40d.lib")
#else
	#pragma comment(lib, "tesseract40.lib")
#endif
