
- ContinueRecog2 사용법

	- config.json 파일을 열어서 환경 설정을 한다.

	- 환경 설정 항목 설명
		- crawl-interval-seconds 항목 : 아직 지원하지 않음

		- name-rect 항목 : investing.com 에서 읽고자하는 회사 이름의 픽셀 영역 위치
			- 사각형 영역의 픽셀 좌표 (left, top, right, bottom)
			- Chrome을 PrintScreen으로 캡쳐한 후, 그림판에서 픽셀 좌표를 계산한다.
			- 이 항목에서 인식된 문자를 파일에 저장한다.

		- stock-rect 항목: investing.com 에서 읽고자하는 주식 정보 픽셀 영역 위치
			- 사각형 영역의 픽셀 좌표 (left, top, right, bottom)
			- Chrome을 PrintScreen으로 캡쳐한 후, 그림판에서 픽셀 좌표를 계산한다.
			- 괄호 안의 문자만 인식해서 저장한다.
			- 괄호가 없으면 무시한다.
			- 이 항목에서 인식된 문자를 파일에 저장한다.

		- offset-y 항목 : investing.com 에서 광고 배너 크기가 달라지는 이유로, 
			name-rect, stock-rect 높이 오프셋 값을 조정하기 위해 사용한다.
			- 기본적으로는 작은 광고 배너일 때, name-rect, stock-rect를 설정하고,
			  큰 광고 배너일 때 offset-y 만큼 rect를 아래로 내려서 문자를 인식한다.

		- wait-site-loading-seconds 항목 : 사이트를 이동 후, 로딩이 완료될 때까지, 대기하는 시간. 초 단위

		- sites 항목 : investing.com 에서 읽고자하는 url과 name을 등록한다. 
			- name 은 name-rect 영역에서 읽고자 하는 회사명이어야 한다. 
			- name 에서 설정된 회사명과 name-rect로 인식한 문자가 다를경우 인식에서 제외된다.
			- name은 간단한 회사명만 설정할 것을 권한다. 
			- 예) Amazon.com Inc (AMZN) 이 아니라 Amazon.com 정도만 설정하자.
			- 영상으로 인식된 문자에서 name에 설정된 문자가 포함되어 있으면 제대로 인식한 것으로 판단한다.

	- 웹 브라우저는 chrome 이 기본으로 설정되어 있어야 한다.

	- 인식하는 도중에 웹브라우저가 꺼져도 다시 띄우게 되어 있다.

	- 특정 상황의 경우 영상인식을 못해서 다음 사이트로 못 넘어가는 경우가 있다. 
	웹브라우저를 끄면 잘되는 경우가 있다. 안되면 처음부터 다시 긁자.

	- 사이트를 모두 읽으면, 텍스트 파일로 결과가 저장되며, 현재 시각으로 파일명이 설정된다.
	엑셀로 읽을 수 있다.

