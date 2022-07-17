#include "Request.h"
//  Coded by Haj4li
// https://github.com/Haj4li/HTTP-Requests-CPP/
void Request::Connect(const char* method, const char* url, const char* page,char* &buffer, const char headers[], DWORD headers_len, const char data[], DWORD data_len)
{
	const char agent[] = "Mozilla/5.0";

	HINTERNET hSession = InternetOpenA(
		agent,
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);
	HINTERNET hConnect = InternetConnectA(
		hSession,
		url,
		80,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		0);
	HINTERNET hHttpFile = HttpOpenRequestA(
		hConnect,
		method,
		page,
		"HTTP/1.1",
		NULL,
		NULL,
		NULL,
		0);
	
	if (headers != NULL)
	{
		HttpAddRequestHeadersA(hHttpFile, headers, headers_len, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	}

	if (!HttpSendRequestA(hHttpFile, headers, headers_len, (char*)data, data_len)) {
		printf("HttpSendRequest error : (%lu)\n", GetLastError());

		InternetErrorDlg(
			GetDesktopWindow(),
			hHttpFile,
			ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED,
			FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
			FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |
			FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
			NULL);

		// close handle 
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return;
	}

	DWORD buffer_size;
	InternetQueryDataAvailable(hHttpFile, &buffer_size, NULL, NULL);
	if (buffer_size == NULL)
	{
		// close handle 
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return;
	}

	buffer = new char[buffer_size + 1];

	while (true) {
		DWORD dwBytesRead;
		BOOL bRead;

		bRead = InternetReadFile(
			hHttpFile,
			buffer,
			buffer_size + 1,
			&dwBytesRead);

		if (dwBytesRead == 0) break;

		if (!bRead) {
			printf("InternetReadFile error : <%lu>\n", GetLastError());
		}
		else {
			buffer[dwBytesRead] = 0;
			//printf("Retrieved %lu data bytes: %s\n", dwBytesRead, buffer);
		}
	}

	InternetCloseHandle(hHttpFile);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);
}
