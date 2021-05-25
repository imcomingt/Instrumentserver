#include "instrumentmodel.hpp"
#include "db.hpp"
unordered_map<string,string> InstrumentModel::use_station(){
    char sql[1024] = "select * from Instrument";
    MySQL mysql;
    unordered_map<string,string> map;
    if (mysql.connect()){
        MYSQL_RES *res = mysql.query(sql); //c查询.query
        if(res != nullptr) {
            MYSQL_ROW row ; //row是**形式 ， char-》char[][]
            while((row = mysql_fetch_row(res)) != nullptr){
                map[row[0]]= row[1];
            }
            mysql_free_result(res);
        }
    }
    return map;
}

void InstrumentModel::use(string instrumentname ,User user){
    char sql[1024] = {0};
    sprintf(sql, "update Instrument set state='%s' where instrumentname='%s'",
            user.getName().c_str(),instrumentname.c_str());
    MySQL mysql;
    if (mysql.connect())
    {
       mysql.update(sql);
        
    }
}

void InstrumentModel::re(string instrumentname){
    char sql[1024] = {0};
    sprintf(sql, "update Instrument set state='nul' where instrumentname='%s'",
            instrumentname.c_str());
    MySQL mysql;
    if (mysql.connect())
    {
       mysql.update(sql);  
    }
}
