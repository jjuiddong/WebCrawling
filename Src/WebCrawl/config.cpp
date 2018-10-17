
#include "stdafx.h"
#include "config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace webcrawl;


cConfig::cConfig()
{
}

cConfig::~cConfig()
{
	Clear();
}


bool cConfig::Read(const char *fileName)
{
	using boost::property_tree::ptree;

	Clear();

	try
	{
		ptree props;
		boost::property_tree::read_json(fileName, props);

		m_crawlInteralSeconds = props.get<int>("crawl-interval-seconds", 100);
		
		string nameR = props.get<string>("name-rect", "0 0 0 0");
		sscanf_s(nameR.c_str(), "%d %d %d %d", &m_nameRect.left, &m_nameRect.top, &m_nameRect.right, &m_nameRect.bottom);

		string stockR = props.get<string>("stock-rect", "0 0 0 0");
		sscanf_s(stockR.c_str(), "%d %d %d %d", &m_stockRect.left, &m_stockRect.top, &m_stockRect.right, &m_stockRect.bottom);

		m_offsetY = props.get<int>("offset-y", 0);
		m_waitSiteLoadingSeconds = props.get<int>("wait-site-loading-seconds", 5);

		auto &sites = props.get_child("sites");
		for (auto &vt : sites)
		{
			sSite site;
			site.url = vt.second.get<string>("url", "");
			site.name = vt.second.get<string>("name", "");
			m_sites.push_back(site);
		}
	}
	catch (std::exception&e)
	{
		m_err = e.what();
		return false;
	}

	return true;
}


void cConfig::Clear()
{
	m_sites.clear();
}
