#ifndef APT_DOCTOR_H
#define APT_DOCTOR_H
#include "User.h"
#include <vector>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void tokenize(string const &str, const char delim, vector<string> &out)
{
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

class Doctor : public User
{
string name;
string surname;
vector<string> prescriptionDrugsPermissions;

public:
    Doctor(string initLine) {
        char delim = ';';
        vector<string> info;
        tokenize(initLine, delim, info);
        setIdentifier(info[2]);
        setLogin(info[0]);
        setPassword(info[1]);
        name = info[3];
        surname = info[4];
        if (info[5] != "-"){
            if (info[5].length() == 4){
                prescriptionDrugsPermissions.push_back(info[5]);
            } else {
                delim = ',';
                info.clear();
                tokenize(info[5], delim, info);
                for (auto & i : info) {
                    prescriptionDrugsPermissions.push_back(i);
                }
            }
        }
    }

    friend ostream & operator<< (ostream &output, const Doctor &s);

    vector<string> getInfo(){
        vector<string> info;
        info.push_back(getIdentifier());
        info.push_back(name);
        info.push_back(surname);
        return info;
    }

    vector<string> getPrescriptionDrugsPermissions(){
        return prescriptionDrugsPermissions;
    }

    ~Doctor() {}
};

ostream & operator<< (ostream &output, const Doctor &d) {
    return output << "Identifier: " << d.identifier << endl << "Login: " << d.login << endl;
}

#endif
