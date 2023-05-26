
#pragma once
#include <windows.h>
#include <wininet.h>
#include <string>
#include <iostream>
#pragma comment (lib, "Wininet.lib")
//  Coded by Haj4li
// https://github.com/Haj4li/HTTP-Requests-CPP/
class Response
{
public:
	DWORD status_code = 0;
	std::string text;
	DWORD error = 0;
};

class Request
{
private:
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hHttpReq;

	DWORD createHandles(const char* server, const char* method, const char* page = NULL);
	void closeHandles();
	Response send();

public:

	INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT;
	std::string user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/109.0";
	std::string proxy = "";

	Response Get(const char* server, const char* page = NULL, const char headers[] = NULL);
	Response Post(const char* server, const char* page, const char headers[], const char* data);

};
