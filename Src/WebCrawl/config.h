//
// 2018-10-16, jjuiddong
// WebCrawling Configuration
//
// JSon Script Sample
//{
   //"crawl-interval-seconds" : "100"
   //, "name-rect" : "0 0 100 100"
   //, "stock-rect" : "0 0 100 100"
   //, "offset-y" : "200"
   //, "wait-loading-site-seconds" : "5"
   //	
   //, "sites" :
   // [
   //	{
   //		"url" : "https://www.investing.com/equities/amazon-com-inc"
   //		, "name" : "Amazon.com Inc (AMZN)"
   //	}

   //	,{
   //		"url" : "https://www.investing.com/equities/netflix,-inc."
   //		, "name" : "Netflix Inc (NFLX)"
   //	}
   // ]
//}
//
#pragma once


namespace webcrawl
{

	class cConfig
	{
	public:
		cConfig();
		virtual ~cConfig();

		bool Read(const char *fileName);
		void Clear();


	public:
		int m_crawlInteralSeconds;
		RECT m_nameRect;
		RECT m_stockRect;
		int m_offsetY;
		int m_waitSiteLoadingSeconds;

		struct sSite {
			string url;
			string name;
		};
		vector<sSite> m_sites;
		string m_err;
	};

}
