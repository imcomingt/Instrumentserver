#ifndef INSTRUMENTMODEL_H
#define INSTRUMENTMODEL_H
#include"user.hpp"
#include"unordered_map"
#include"vector"
#include"string"
using namespace std;
class InstrumentModel{
public:
    unordered_map<string,string> use_station();

    void use(string name ,User user);

    void re(string name);
};
#endif