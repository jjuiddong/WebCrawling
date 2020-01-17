
#include <iostream>
#include <string>
#include <fstream>
#include "gumbo.h"
#include <curl/curl.h>
#pragma comment(lib, "libcurl.lib")

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//  libcurl write callback function
int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
	if (writerData == NULL)
		return 0;
	writerData->append(data, size*nmemb);
	return size * nmemb;
}

//  libcurl connection initialization
static bool init(CURL *&conn, char *url)
{
	CURLcode code;

	conn = curl_easy_init();

	if (conn == NULL) {
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_SSL_VERIFYPEER, 0L);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_SSL_VERIFYHOST, 0L);
	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}

	//code = curl_easy_setopt(conn, CURLOPT_TIMECONDITION, 10);
	//if (code != CURLE_OK) {
	//	fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
	//	return false;
	//}

	return true;
}


void search_for_links(GumboNode* node) 
{
	if (node->type != GUMBO_NODE_ELEMENT)
		return;

	GumboAttribute* href;
	if (node->v.element.tag == GUMBO_TAG_DIV)
	{
		if (href = gumbo_get_attribute(&node->v.element.attributes, "id")) {
			//std::cout << href->value << std::endl;
			if (std::string::npos != std::string(href->value).find("description"))
			{
				std::cout << href->value << std::endl;
			}
		}
	}
	GumboVector* children = &node->v.element.children;
	for (unsigned int i = 0; i < children->length; ++i) {
		search_for_links(static_cast<GumboNode*>(children->data[i]));
	}
}


int main(int argc, char *argv[])
{
	CURL *conn = NULL;
	CURLcode code;
	std::string title;

	char url[] = "https://kmong.com/gig/78513";

	curl_global_init(CURL_GLOBAL_DEFAULT);
	if (!init(conn, url))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);
	if (code != CURLE_OK) 
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}

	GumboOutput* output = gumbo_parse(buffer.c_str());
	search_for_links(output->root);
	gumbo_destroy_output(&kGumboDefaultOptions, output);

	std::ofstream ofs("html2.txt");
	if (ofs.is_open())
	{
		ofs << buffer << std::endl;
	}

	return EXIT_SUCCESS;
}

