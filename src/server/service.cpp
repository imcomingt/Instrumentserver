#include "service.hpp"
#include "public.hpp"
#include "muduo/base/Logging.h"
#include "vector"
#include <map>
#include "redis.hpp"
using namespace std;
using namespace muduo;
Service *Service::instance() //给出一个接口，让网络层可以用它的公有属性
{
    static Service service;
    return &service;
}

Service::Service()
{ //回调函数 放的map  信号类型放到Map里了， 然后id号对应回调函数 //这里最重要，是个网络层和下面层的接口

    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&Service::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&Service::reg, this, _1, _2, _3)});
    _msgHandlerMap.insert({INSTRUMENTQUARY_MSG, std::bind(&Service::instrumentQaruy, this, _1, _2, _3)});
    _msgHandlerMap.insert({INSTRUMENT_MSG, std::bind(&Service::instrumentUse, this, _1, _2, _3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&Service::oneChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&Service::addFriend, this, _1, _2, _3)});
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&Service::createGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&Service::addGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&Service::groupChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({LOGINOUT_MSG, std::bind(&Service::loginout, this, _1, _2, _3)});
    if(_redis.connect()){
        _redis.init_notify_handler(std::bind(&Service::handleRedisSubscribeMessage, this, _1, _2));
    }
}
void Service::reset(){
    //把状态转下线
    _userModel.resetState();
}
MsgHandler Service::getHandler(int msgid)
{ //返回回调函数 通过map和回调绑定在了一起 这个消息id ，
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end())
    {
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp) { //lamuda
            LOG_ERROR << "msgid:" << msgid << "can not find hander!";
        };
    }
    else
        return _msgHandlerMap[msgid];
}
void Service::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"].get<int>();
    string pwd = js["password"];
    User user = _userModel.query(id);
    if (user.getId() == id && user.getPwd() == pwd)
    {
        if (user.getState() == "online")
        {
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["error"] = 2;
            response["errmsg"] = "用户已经登陆";
            conn->send(response.dump());
        }
        else
        {
            {
                lock_guard<mutex> lock(_connMutex); //lock_guard 构造是加锁，析构是解锁
                _userConnMap.insert({id, conn});
            } //加锁
            _redis.subscribe(id); 
            user.setState("online");
            _userModel.updataState(user);
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["id"] = user.getId();
            response["name"] = user.getName();
            response["error"] = 0;
            //离线消息发送
            vector<string> vec = _offlineMsgModel.quary(id);
            if(!vec.empty()) {
                response["offlinemsg"] = vec;
                _offlineMsgModel.remove(id);
            }
            vector<User> userVec = _friendModel.quary(id);
            if (!userVec.empty()){
                vector<string> vec1;
                for (User &user : userVec) {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec1.push_back(js.dump());
                }
                response["friends"] = vec1;
            }  //加个group
            conn->send(response.dump());
        }
    }
    else
    {
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["error"] = 1;
        response["errmsg"] = "密码错误";
        conn->send(response.dump());
    }
}
void Service::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _userModel.insert(user);
    if (state)
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["id"] = user.getId();
        response["error"] = 0;
        conn->send(response.dump());
    }
    else
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["error"] = 1;
        conn->send(response.dump());
    }
}
void Service::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(userid);
        if (it != _userConnMap.end()) {
            _userConnMap.erase(it);
        }
    }
    _redis.unsubscribe(userid); 
    User user(userid,"","","offline");    //把状态转成off
    _userModel.updataState(user); 
}
void Service::clientCloseException(const TcpConnectionPtr &conn) //异常退出
{
    User user;
    {
        lock_guard<mutex> lock(_connMutex); //lock_guard 保证线程安全

        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it) //找存在的连接
        {
            if (it->second == conn)
            {
                user.setId(it->first); //找到后设置id
                _userConnMap.erase(it);
                break;
            }
        } 
    }
    if (user.getId() != -1)
    {  
        _redis.unsubscribe(user.getId());
        user.setState("offline");     //把状态转成off
        _userModel.updataState(user); //mysql 更新状态
    }
}

void Service::instrumentQaruy(const TcpConnectionPtr &conn, json &js, Timestamp time){
    json respones;
    respones["instrument"] = _instrumentModel.use_station();
    conn->send(respones.dump());
}
void Service::instrumentUse(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int re = js["re"].get<int>();
    string strument = js["strumentname"];
    if(re){
        _instrumentModel.re(strument);
        return ;
    }
    int id  = js["id"].get<int>();
    string name = js["name"];
    User user;
    user.setId(id);
    user.setName(name);
    _instrumentModel.use(strument,user);
    
}
void Service::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{   //int id  = js["id"].get<int>();
    //int name = js["name"].get<int>();
    int toid = js["toid"].get<int>();
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end())
        {
            //在线
            it->second->send(js.dump()); //消息中转 ，
            return;
        }
    }
    User user = _userModel.query(toid);
    if (user.getState()=="online") {
         _redis.publish(toid, js.dump());
         return;
    }
    _offlineMsgModel.insert(toid,js.dump());
}

void Service::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();
    _friendModel.insert(userid, friendid);

}

void Service::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    string name = js["groupname"];
    string desc = js["groupdesc"];

    // 存储新创建的群组信息
    Group group(-1, name, desc);
    if (_groupModel.createGroup(group))
    {
        // 存储群组创建人信息
        _groupModel.addGroup(userid, group.getId(), "creator");
    }
}

void Service::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    _groupModel.addGroup(userid, groupid, "normal");
}

void Service::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = _groupModel.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(_connMutex);
    for (int id : useridVec)
    {
        auto it = _userConnMap.find(id);//在线用户
        if (it != _userConnMap.end())
        {
            // 转发群消息
            it->second->send(js.dump());//直接点对点发生
        }
        else
        {   User user = _userModel.query(id);
            if(user.getState()=="online"){
                _redis.publish(id, js.dump());
            }else _offlineMsgModel.insert(id, js.dump());
        }
    }
}
void Service::handleRedisSubscribeMessage(int userid, string msg) //redis中获取订阅消息
{
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnMap.find(userid);
    if (it != _userConnMap.end())
    {
        it->second->send(msg);
        return;
    }
    _offlineMsgModel.insert(userid, msg);
}

