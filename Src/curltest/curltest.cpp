
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <curl/curl.h>
#pragma comment(lib, "libcurl.lib")

//  libcurl variables for error strings and returned data
static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//  libcurl write callback function
static int writer(char *data, size_t size, size_t nmemb,
	std::string *writerData)
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

	return true;
}


int main(int argc, char *argv[])
{
	CURL *conn = NULL;
	CURLcode code;
	std::string title;

	//char url[] = "https://www.naver.com/";
	char url[] = "https://cinrueom.tistory.com/10";
	//char url[] = "http://www.jjuiddong.co.kr/wiki";

	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection
	if (!init(conn, url)) 
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}

	// Retrieve content for the URL
	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK) {
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}

	std::ofstream ofs("html.txt");
	if (ofs.is_open())
	{
		ofs << buffer << std::endl;
	}

	return EXIT_SUCCESS;
}

