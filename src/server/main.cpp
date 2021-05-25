#include"server.hpp"
#include"service.hpp"
#include<iostream>
#include<signal.h>
using namespace std;
void resetHandler(int){ //int 
    Service::instance()->reset();
    exit(0);
}
int main(int argc, char* argv[]) {
    if(argc < 3) {
        cerr << "./server xxx.xxx.xxx.x port" <<endl;
        return 0 ;
    }
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    signal(SIGINT, resetHandler);//ctrl + c 信号
    EventLoop loop;
    InetAddress addr(ip, port);
    Server server(&loop, addr, "server");

    server.start();
    loop.loop();

    return 0;
}