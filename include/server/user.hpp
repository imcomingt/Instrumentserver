#ifndef USER_H
#define USER_H

#include<string>
using namespace std;

//对应user表 ORM类 映射关系类
class User { //关系映射 ， 用User类包括数据库中的User类
public:
    User(int id = -1, string name = "", string pwd = "", string state = "offline"){
        this->id = id;
        this->name = name;
        this->password = pwd;
        this->state = state;
    }
    void setId(int id) {this->id = id;}
    void setName(string name) {this->name = name;}
    void setPwd(string pwd) {this->password = pwd;}
    void setState(string state) {this->state = state;}

    int getId() {return this->id;}
    string getName() {return this->name;}
    string getPwd() {return this->password;}
    string getState() {return this->state;}

private:
    int id;
    string name;
    string password;
    string state;
};
#endif