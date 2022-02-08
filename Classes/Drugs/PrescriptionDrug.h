#ifndef APT_PRESCRIPTION_DRUG_H
#define APT_PRESCRIPTION_DRUG_H
#include "Drug.h"
#include <string>
#include <vector>

using namespace std;

class PrescriptionDrug : public Drug {
    vector<string> doctorIdentifiers;
public:
    vector<string> getDoctorIdentifiers(){ return doctorIdentifiers; }
    PrescriptionDrug(string initLine) : Drug(initLine){
        char delim = ';';
        vector<string> info;
        tokenize(initLine, delim, info);

        string s = info[info.size() - 1];
        if ( s.find(',') != string::npos ) {
            info.clear();
            delim = ',';
            tokenize(s, delim, info);
            for (auto &i: info) {
                doctorIdentifiers.push_back(i);
            }
        } else doctorIdentifiers.push_back(info[info.size() - 1]);
    }
    ~PrescriptionDrug(){}
};

#endif
