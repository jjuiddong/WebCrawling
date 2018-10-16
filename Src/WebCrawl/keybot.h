//
// 2018-10-16, jjuiddong
// keyboard event
//
#pragma once


namespace webcrawl
{

	class cKeyBot
	{
	public:
		cKeyBot();
		void KeyUp(char key);
		void KeyDown(char key);
		void KeyClick(char key);

	private:
		INPUT _buffer[1];
	};

}
