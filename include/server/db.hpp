#ifndef DB_H
#define DB_H
// 数据库配置信息 
#include<string>
#include<mysql/mysql.h>
using namespace  std;
     
 // 数据库操作类 
class MySQL {
public: 
// 初始化数据库连接 
    MySQL();
// 释放数据库连接资源
    ~MySQL();
     // 连接数据库
    bool connect();
    bool update(string sql);
    MYSQL_RES* query(string sql);
    MYSQL* getConnect();
    MYSQL* getconnection();
private: MYSQL *_conn; 
};
#endif