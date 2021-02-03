#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "httplib.h"

void test(const httplib::Request& req, httplib::Response& res)
{
    (void)req;

    printf("begin\n");
    res.set_header("Set-Cookie", "JSESSIONID=123123");
    printf("end\n");
}

int main()
{
    httplib::Server ser;
    //ser.Get("/a", [传递给表达式中的变量](函数的参数){具体的实现});
    ser.Get("/aaa", test);
    ser.Get("/a", [](const httplib::Request& req, httplib::Response& res){
            (void)req;
            printf("heheheheh\n");
            res.set_header("Content-Type", "application/json");
            res.set_header("set-Cookie", "JSESSIONID=yyyyy");
            });

    ser.Get("/b", [](const httplib::Request& req, httplib::Response& res){
            printf("aaaaaaaaaaaaa\n");
            std::cout << req.get_header_value("Cookie") << std::endl;
            });
    ser.set_mount_point("/", "./www");
    ser.listen("0.0.0.0", 16666);
    return 0;
}
