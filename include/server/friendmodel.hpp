#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H
#include"user.hpp"
#include<vector>
using namespace std;

class FriendModel{
public:
    void insert(int userid, int friendid);

    vector<User> quary(int userid);
};
#endif