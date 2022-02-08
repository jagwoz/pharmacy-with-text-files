#ifndef APT_DRUG_H
#define APT_DRUG_H
#include <string>

using namespace std;

class Drug {
    string identifier;
    string name;
    string description;
    int amount;
    float price;
    bool prescription;

protected:
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

public:
    string getIdentifier(){ return identifier; }
    string getName(){ return name; }
    string getDescription(){ return description; }
    int getAmount(){ return amount; }
    float getPrice(){ return price; }
    bool isPrescription(){ return prescription; }
    void setAmount(int a){ amount -= a; }

    Drug(string initLine){
        char delim = ';';
        vector<string> info;
        tokenize(initLine, delim, info);
        identifier = info[0];
        name = info[1];
        description = info[2];
        if (info[3] == "1")
            prescription = true;
        else prescription = false;
        amount = stoi(info[4]);
        price = stof(info[5]);
    }
    ~Drug() {}
};

#endif
