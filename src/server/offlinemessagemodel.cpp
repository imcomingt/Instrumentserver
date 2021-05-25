
#include "offlinemessagemodel.hpp"
#include "db.hpp"

void OfflineMsgModel::insert(int userid, string msg)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into OfflineMessage values('%d','%s')",
            userid,msg.c_str());
    MySQL mysql;
    if (mysql.connect())
    {
       mysql.update(sql);
        
    }
}

void OfflineMsgModel::remove(int userid){
     char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid = %d",
            userid);
    MySQL mysql;
    if (mysql.connect())
    {
       mysql.update(sql);
        
    }
}

vector<string> OfflineMsgModel::quary(int userid){
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid = %d",userid);
    MySQL mysql;
    vector<string> vec;
    if (mysql.connect()){
        MYSQL_RES *res = mysql.query(sql); //c查询.query
        if(res != nullptr) {
            MYSQL_ROW row ; //row是**形式 ， char-》char[][]
            while((row = mysql_fetch_row(res)) != nullptr){
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
        }
    }
    return vec;
}