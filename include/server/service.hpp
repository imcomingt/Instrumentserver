#ifndef SERVICE_H
#define SERVICE_H
#include<muduo/net/TcpServer.h>
#include<unordered_map>
#include"json.hpp"
#include<functional>
#include"usermodel.hpp"
#include<mutex>
#include"offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include"groupmodel.hpp"
#include"redis.hpp"
#include"instrumentmodel.hpp"
using namespace std;
using namespace muduo::net;
using namespace muduo;
using json =nlohmann::json;

using  MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

class Service { //连接 网络和业务的中间模块
public:

    static Service* instance();
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);  //回调函数的具体类型
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    MsgHandler getHandler(int msgid);
    void clientCloseException(const TcpConnectionPtr &conn);  //异常退出
    void instrumentQaruy(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void instrumentUse(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void reset();
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void handleRedisSubscribeMessage(int, string);
private:
    Service();
    unordered_map<int, MsgHandler> _msgHandlerMap;
    UserModel _userModel;
    //存在线用户的通信链接
    InstrumentModel _instrumentModel;
    unordered_map<int,TcpConnectionPtr> _userConnMap;
    mutex _connMutex;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;
    Redis _redis;

    
};

#endif