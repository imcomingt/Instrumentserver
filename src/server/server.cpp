#include"server.hpp"
#include<functional>
#include<string.h>
#include"json.hpp"
#include"service.hpp"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

Server::Server(EventLoop* loop,   //构造， 把回调函数设定好
            const InetAddress& listenAddr,
            const string& nameArg)
                : _server(loop, listenAddr, nameArg), _loop(loop)
{      _server.setConnectionCallback(std::bind(&Server::onConnection, this, _1));
       _server.setMessageCallback(std::bind(&Server::onMessage, this, _1, _2, _3));
       _server.setThreadNum(4);  //线程数
}    
void Server::start(){  //start
    _server.start();
}    
void Server::onConnection(const TcpConnectionPtr &conn){  //连接回调 ， 
    if(!conn->connected()){   //异常退出有问题， //原来是机制有问题， 不是错误退出。quit 居然才是，
        Service::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}
void Server::onMessage(const TcpConnectionPtr &conn,
                Buffer* buffer,
                Timestamp time){
    string buf = buffer->retrieveAllAsString(); //接受buff 
    json js = json::parse(buf);  //转成js格式

    MsgHandler msgHandler = Service::instance()->getHandler(js["msgid"].get<int>()); //处理根据msgid处理 ，就是信号类型
    msgHandler(conn, js, time); //调用设定好的回调函数 ，ok ，双重回调
 }