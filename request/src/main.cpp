#include "Request.h"
//  Coded by Haj4li
// https://github.com/Haj4li/HTTP-Requests-CPP/
int main()
{
    const char headers[] = "Content-Type: application/x-www-form-urlencoded;Kos: yes";
    Request req;
    req.user_agent = "test";
   
    Response resp = req.Get("google.com", "");
    std::cout << resp.text << std::endl;
    std::cout << "Last Error : " << resp.error << std::endl;
    std::cout << "Statue Code : " << resp.status_code << std::endl;
}
