#pragma once
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
//  Coded by Haj4li
#pragma comment (lib, "Wininet.lib")

class Request
{

	public:
	
	void Connect(const char* method, const char* url, const char* page, char* &buffer,const char headers[] = NULL,DWORD headers_len = NULL, const char data[] = NULL, DWORD data_len = NULL);
		
};

