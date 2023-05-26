#include "Request.h"
//  Coded by Haj4li
// https://github.com/Haj4li/HTTP-Requests-CPP/
DWORD Request::createHandles(const char* server, const char* method, const char* page)
{
	hSession = InternetOpenA(user_agent.c_str(), (proxy.length() == 0) ? INTERNET_OPEN_TYPE_DIRECT : INTERNET_OPEN_TYPE_PROXY, (proxy.length() == 0) ? NULL : proxy.c_str(), NULL, 0);
	if (hSession == NULL)
		return GetLastError();

	hConnect = InternetConnectA(hSession, server, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hConnect == NULL)
	{
		closeHandles();
		return GetLastError();
	}

	hHttpReq = HttpOpenRequestA(hConnect, method, page, "HTTP/1.1", NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (hHttpReq == NULL)
	{
		closeHandles();
		return GetLastError();
	}
	return 0;
}

void Request::closeHandles()
{
	if (hConnect)
		InternetCloseHandle(hConnect);
	if (hSession)
		InternetCloseHandle(hSession);
	if (hHttpReq)
		InternetCloseHandle(hHttpReq);
}

Response Request::send()
{
	Response resp;
	DWORD length = sizeof(DWORD);

	HttpQueryInfoA(
		hHttpReq,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&resp.status_code,
		&length,
		NULL
	);
	DWORD resSize;
	InternetQueryDataAvailable(hHttpReq, &resSize, NULL, NULL);
	if (resSize == NULL)
	{
		resp.error = GetLastError();
		closeHandles();
		return resp;
	}
	resp.text.clear();
	resp.text = "";
	DWORD dwBytesRead;
	char buffer[1024];
	do
	{
		if (InternetReadFile(hHttpReq, &buffer, resSize + 1, &dwBytesRead)) {
			if (dwBytesRead != 0)
			{
				resp.text.append(buffer, dwBytesRead);
			}
		}
	} while (dwBytesRead != 0);
	closeHandles();
	return resp;
}

Response Request::Get(const char* server, const char* page, const char headers[])
{
	Response resp;
	DWORD hs = NULL;
	resp.error = createHandles(server, "GET", page);
	if (resp.error != 0)
		return resp;
		
	if (headers != NULL)
	{
		hs = std::string(headers).length();
		HttpAddRequestHeadersA(hHttpReq, headers, hs, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	}
	if (!HttpSendRequestA(hHttpReq, headers, hs, NULL, 0))
	{
		resp.error = GetLastError();
		closeHandles();
		return resp;
	}
	return send();
}

Response Request::Post(const char* server, const char* page, const char headers[], const char* data)
{
	Response resp;
	DWORD hs= NULL;
	resp.error = createHandles(server, "POST", page);
	if (resp.error != 0)
		return resp;
	if (headers != NULL)
	{
		hs = std::string(headers).length();
		HttpAddRequestHeadersA(hHttpReq, headers, hs, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	}

	if (!HttpSendRequestA(hHttpReq, headers, hs, (char*)data, std::string(data).length()))
	{
		resp.error = GetLastError();
		closeHandles();
		return resp;
	}
	return send();
}
