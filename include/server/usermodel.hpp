#ifndef USERMODEL_H
#define USERMODEL_H

#include"user.hpp"

class UserModel{  //用户的动作 //业务模块
    public: 
        bool insert(User &user);

        User query(int id); 

        bool updataState(User user);

        void resetState();
};
#endif