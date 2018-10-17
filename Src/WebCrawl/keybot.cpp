
#include "stdafx.h"
#include "KeyBot.h"

using namespace webcrawl;


cKeyBot::cKeyBot()
{
	_buffer->type = INPUT_KEYBOARD;
	_buffer->ki.wScan = 0;
	_buffer->ki.time = 0;
	_buffer->ki.dwExtraInfo = 0;
}

void cKeyBot::KeyUp(char key)
{
	_buffer->ki.wVk = key;
	_buffer->ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, _buffer, sizeof(INPUT));
}

void cKeyBot::KeyDown(char key)
{
	_buffer->ki.wVk = key;
	_buffer->ki.dwFlags = 0;
	SendInput(1, _buffer, sizeof(INPUT));
}

void cKeyBot::KeyClick(char key)
{
	KeyDown(key);
	Sleep(20);
	KeyUp(key);
}
