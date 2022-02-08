#ifndef APT_USER_H
#define APT_USER_H
#include <string>
#include <vector>

using namespace std;

class User
{
    string password;
protected:
    string identifier;
    string login;
public:
    void setIdentifier(string i){ identifier = i; }
    void setLogin(string l){ login = l; }
    void setPassword(string p){ password = p; }
    string getIdentifier(){ return identifier; }
    string getLogin(){ return login; }
    string getPassword(){ return password; }
    User() {}
    virtual vector<string> getInfo(){}
    virtual ~User() = 0;
};
User::~User() {}

#endif
