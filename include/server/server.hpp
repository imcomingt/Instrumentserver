#ifndef SERVER_H
#define SERVER_H

#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;
class Server { //网络模块
public:
    Server(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);
    void start();
private:
    void onConnection(const TcpConnectionPtr&); //连接回调函数
    void onMessage(const TcpConnectionPtr&,    //buffer回调函数
                    Buffer*,
                    Timestamp);
    TcpServer _server;  
    EventLoop *_loop;
};

#endif