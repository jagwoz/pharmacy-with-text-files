#ifndef APT_CLIENT_H
#define APT_CLIENT_H
#include "User.h"
#include <string>

class Client : public User
{
    friend class Doctor;
    friend vector<string> getPurchasesHistory(Client &c);
    vector<string> purchasesHistory;
    vector<string> prescriptions;

public:
    Client(string initLine) {
        char delim = ';';
        vector<string> info;
        tokenize(initLine, delim, info);
        setIdentifier(info[2]);
        setLogin(info[0]);
        setPassword(info[1]);

        vector<string> prescripts;
        if (info[3] != "-"){
            delim = ',';
            tokenize(info[3], delim, prescripts);
            for (auto & i : prescripts) {
                prescriptions.push_back(i);
            }
        }

        for (int i = 4; i < info.size(); i++){
            vector<string> buy;
            if (info[i] != "-"){
                delim = ',';
                buy.clear();
                tokenize(info[i], delim, buy);
                for (auto & i : buy) {
                    purchasesHistory.push_back(i);
                }
            }
        }
    }
    vector<string> getPrescriptions(){ return prescriptions; }
    void addPrescription(string p){ prescriptions.push_back(p); }
    void addPurchaseHistory(vector<string> ph){
        for (auto &p: ph) {
            purchasesHistory.push_back(p);
        }
    }
    bool hasPrescription(string s){
        for (auto &p: prescriptions) {
            if (p == s) {
                return true;
            }
        }
        return false;
    }
    void removePrescription(string s){
        vector<string> newPrescriptions;
        for (auto &p: prescriptions) {
            if (p != s) {
                newPrescriptions.push_back(p);
            }
        }
        prescriptions = newPrescriptions;
    }
    vector<string> getInfo(){
        vector<string> info;
        info.push_back(getIdentifier());
        return info;
    }
    ~Client() {}
};

vector<string> getPurchasesHistory(Client & c)
{
    return c.purchasesHistory;
}

#endif
