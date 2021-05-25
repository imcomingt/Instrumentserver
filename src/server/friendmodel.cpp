#include "friendmodel.hpp"
#include "db.hpp"
void FriendModel::insert(int userid, int friendid)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend values('%d','%d')",
            userid,friendid);
    MySQL mysql;
    if (mysql.connect())
    {
       mysql.update(sql);
        
    }
}

vector<User> FriendModel::quary(int userid)
{
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.name,a.state from User a inner join Friend b on b.friendid = a.id where b.userid = %d",userid);
    MySQL mysql;
    vector<User> vec;
    if (mysql.connect()){
        MYSQL_RES *res = mysql.query(sql); //c查询.query
        if(res != nullptr) {
            MYSQL_ROW row ; //row是**形式 ， char-》char[][]
            while((row = mysql_fetch_row(res)) != nullptr){
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}