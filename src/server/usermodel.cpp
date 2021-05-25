#include"usermodel.hpp"
#include"db.hpp"
#include"iostream"
using namespace std;

bool UserModel::insert(User &user){
    char sql[1024] = {0};
    sprintf(sql, "insert into User(name, password, state) values('%s','%s','%s')",
        user.getName().c_str(),user.getPwd().c_str(),user.getState().c_str());
    MySQL mysql;
    if (mysql.connect()){
        if(mysql.update(sql)) { 
            user.setId(mysql_insert_id(mysql.getConnect()));  //给user ， mysql_insert_id返回自动生成的ID号
            return true;
        }
    }
    return false;
}
User UserModel::query(int id){ 
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d",id);
    MySQL mysql;
    if (mysql.connect()){
        MYSQL_RES *res = mysql.query(sql); //c查询.query
        if(res != nullptr) {
            MYSQL_ROW row = mysql_fetch_row(res); //row是**形式 ， char-》char[][]
            if (row != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);  //释放mysql的资源
                return user;   //请求 ，查询， 是MYSQL_ROW 类型， 
            } 
        }
    }
    return User();
}
bool UserModel::updataState(User user){
    char sql[1024] = {0};
    
    sprintf(sql, "update User set state = '%s' where id = %d",  //mysql的命令 
        user.getState().c_str(),user.getId());
    MySQL mysql;  
    if (mysql.connect()){   //mysql连接 ， 然后update
        if(mysql.update(sql)) {
            return true;
        }
    }
    return false;
}
void UserModel::resetState(){
    char sql[1024] = "update User set state = 'offline' where state = 'online'"; //有点问题
    
    MySQL mysql;  
    if (mysql.connect()){   //mysql连接 ， 然后update
        mysql.update(sql) ;    
    }
}