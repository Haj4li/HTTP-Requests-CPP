#include "Request.h"
//  Coded by Haj4li
// https://github.com/Haj4li/HTTP-Requests-CPP/
int main(int argc, char* argv[]) {

	const char ip[] = "127.0.0.1";

	Request req;
	char* buf = NULL;
	const char headers[] = "Content-Type: application/x-www-form-urlencoded";
	const char post[] = "data=test";

	req.Connect("GET", ip, "test.php", buf,headers, sizeof(headers)-1);
	printf("\nGET : %s\n", buf);
	free(buf);

	req.Connect("POST", ip, "test.php", buf, headers, sizeof(headers)-1,post,sizeof(post)-1);
	printf("\nPOST : %s\n", buf);
    
	free(buf);

	return 0;
}
