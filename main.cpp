#include "Classes/Doctor.h"
#include "Classes/Client.h"
#include "Classes/Drugs/Drug.h"
#include "Classes/Drugs/PrescriptionDrug.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <typeinfo>
#include <regex>

#define MAX_PRESCRIPTIONS 2
typedef vector<Doctor> doctors_list;
typedef vector<Client> clients_list;
typedef vector<Drug> drugs_list;
typedef vector<PrescriptionDrug> pre_drugs_list;

using namespace std;

int main()
{
    string doctorsFilePath = "Files/doctors.txt";
    string drugsFilePath = "Files/drugs.txt";
    string clientsFilePath = "Files/clients.txt";
    string tmpFilePath = "Files/tmp.txt";
    string date = "31.01.2022";

    doctors_list doctors;
    clients_list clients;
    drugs_list drugs;
    pre_drugs_list prescriptionDrugs;

    ifstream doc( doctorsFilePath );
        string line;
        while( getline( doc, line ) ) {
            Doctor doctor(line);
            doctors.push_back( doctor );
        }
    doc.close();

    ifstream cli( clientsFilePath );
    while( getline( cli, line ) ) {
        Client client(line);
        clients.push_back( client );
    }
    cli.close();

    ifstream dru( drugsFilePath );
    while( getline( dru, line ) ) {
        if ( line.find(";1;") != string::npos ){
            PrescriptionDrug drug(line);
            prescriptionDrugs.push_back( drug );
        } else {
            Drug drug(line);
            drugs.push_back( drug );
        }
    }
    dru.close();

    ofstream tmp( tmpFilePath );
    tmp.close();

    char decision;
    string login, password, drugNumber, drugAmount, toChange, toChange2;
    bool validLogin = false, isEmpty = true;
    vector<string> ph, nf;
    float fullPrice;
    int userId, i, nr;

    while(true){
        cout << "1. Doctor login." << endl << "2. Client login." << endl << "3. Exit." << endl;
        cin >> decision;

        switch (decision){
            case '1':
                cout << "Login: "; cin >> login;
                cout << "Password: "; cin >> password;
                userId = 0;
                for (auto & d : doctors) {
                    if (d.getLogin() == login and d.getPassword() == password){
                        validLogin = true; break;
                    } userId ++;
                }
                if (!validLogin) { decision = '3'; break; }

                cout << endl;
                cout << doctors[userId];

                while(true) {
                    cout << endl << "1. Write prescription." << endl << "2. Purchase history." << endl << "3. Warehouse."
                         << endl << "4. Logout." << endl;
                    cin >> decision;

                    switch (decision) {
                        case '1':
                            i = 0;
                            isEmpty = true;
                            cout << endl << "Prescriptions possible to write:" << endl;
                            ph.clear();
                            for (auto &d: doctors[userId].getPrescriptionDrugsPermissions()) {
                                for (auto &e: prescriptionDrugs) {
                                    for (auto &f: e.getDoctorIdentifiers()) {
                                        if (e.getIdentifier() == d && f == doctors[userId].getIdentifier() ) {
                                            isEmpty = false;
                                            ph.push_back(e.getIdentifier());
                                            cout << ++i << ".\t id - " << e.getIdentifier() << "\t name - "
                                                 << e.getName()
                                                 << " description - " << e.getDescription() << endl;
                                        }
                                    }
                                }
                            }
                            if (isEmpty) cout << "You have not permission to write any prescriptions." << endl;
                            cout << endl << "Chose prescription number:" << endl;
                            cin >> drugNumber;
                            if (stoi(drugNumber) < 1 || stoi(drugNumber) > i) break;

                            i = 0;
                            cout << endl << "Clients:" << endl;
                            for (auto &d: clients) {
                                cout << ++i << ".\t id - " << d.getIdentifier() << "\t username - " << d.getLogin() << endl;
                            }
                            cout << endl << "Chose client to write prescription:" << endl;
                            cin >> drugAmount;
                            if (stoi(drugAmount) < 1 || stoi(drugAmount) > i) break;

                            if (!clients[stoi(drugAmount) - 1].hasPrescription(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier()) && clients[stoi(drugAmount) - 1].getPrescriptions().size() < MAX_PRESCRIPTIONS){
                                clients[stoi(drugAmount) - 1].addPrescription(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier());
                                cli.open( clientsFilePath );
                                tmp.open( tmpFilePath );
                                while( getline( cli, line ) ) {
                                    if ( line.find(clients[stoi(drugAmount) - 1].getIdentifier()) == string::npos ){
                                        tmp << line << endl;
                                    } else {
                                        toChange = line;
                                        if ( toChange.find(clients[stoi(drugAmount) - 1].getIdentifier() + ";-;") != string::npos ){
                                            toChange = regex_replace(toChange,
                                                                     regex("" + clients[stoi(drugAmount) - 1].getIdentifier() + ";-;"),
                                                                     "" + clients[stoi(drugAmount) - 1].getIdentifier() + ";" + ph[stoi(drugNumber) - 1] + ";");
                                        } else {
                                            toChange = regex_replace(toChange,
                                                                     regex("" + clients[stoi(drugAmount) - 1].getIdentifier() + ";"),
                                                                     "" + clients[stoi(drugAmount) - 1].getIdentifier() + ";" + ph[stoi(drugNumber) - 1] + ",");
                                        }
                                        tmp << toChange << endl;
                                    }
                                }
                                cli.close();
                                tmp.close();
                                remove("Files/clients.txt");
                                rename("Files/tmp.txt", "Files/clients.txt");

                                cout << "Prescription written" << endl;
                            } else cout << "Client have this prexcription or max number of prescriptions." << endl;

                            break;
                        case '2':
                            isEmpty = true;
                            cout << endl << "Purchases history:" << endl;
                            nr = 0;

                            for (int c = 0; c < clients.size(); c++) {
                                i = 0;
                                ph = getPurchasesHistory(clients[c]);
                                for (int j = 0; j < ph.size() / 5; j++) {
                                    isEmpty = false;
                                    cout << ++nr << ".\t client id - " << clients[c].getIdentifier() << "\t date - " << ph[i * 5] << "\t drug id - " << ph[i * 5 + 1]
                                         << "\t prescription - ";
                                    if (ph[i * 5 + 2] == "1") cout << "true \t";
                                    else cout << "false \t";
                                    cout << " amount - " << ph[i * 5 + 3] << "\t full price - " << ph[i * 5 + 4]
                                         << endl;
                                    i++;
                                }
                            }
                            if (isEmpty) cout << "Purchases history is empty." << endl;
                            break;
                        case '3':
                            i = 0;
                            cout << endl << "Warehouse:\nDrugs:" << endl;
                            for (auto &d: drugs) {
                                cout << ++i << ".\t id - " << d.getIdentifier() << "\t name - " << d.getName()
                                     << " description - " << d.getDescription()
                                     << " amount - " << d.getAmount() << "\t price/1 - " << d.getPrice() << endl;
                            }
                            i = 0;
                            cout << "Prescription drugs:" << endl;
                            for (auto &d: prescriptionDrugs) {
                                cout << ++i << ".\t id - " << d.getIdentifier() << "\t name - " << d.getName()
                                     << " description - " << d.getDescription()
                                     << " amount - " << d.getAmount() << "\t price/1 - " << d.getPrice() << endl;
                            }
                    }
                    if (decision == '4') { validLogin = false; break; }
                }

                break;
            case '2':
                cout << "Login: "; cin >> login;
                cout << "Password: "; cin >> password;

                userId = 0;
                for (auto & c : clients) {
                    if (c.getLogin() == login and c.getPassword() == password){
                        validLogin = true; break;
                    } userId ++;
                }
                if (!validLogin) { decision = '3'; break; }

                while(true) {
                    cout << endl << "1. Buy drugs." << endl << "2. Buy prescriptions drugs." << endl << "3. My prescriptions." <<
                    endl << "4. My purchase history." << endl << "5. Logout." << endl;
                    cin >> decision;

                    switch (decision) {
                        case '1':
                            i = 0;
                            cout << endl << "Drugs:" << endl;
                            for (auto &d: drugs) {
                                cout << ++i << ".\t id - " << d.getIdentifier() << "\t name - " << d.getName()
                                     << " description - " << d.getDescription()
                                     << " amount - " << d.getAmount() << "\t price/1 - " << d.getPrice() << endl;
                            }
                            cout << endl << "Chose drug number:" << endl;
                            cin >> drugNumber;
                            if (stoi(drugNumber) < 1 || stoi(drugNumber) > i) break;
                            cout << "Chose drug amount:" << endl;
                            cin >> drugAmount;
                            if (stoi(drugAmount) < 1 || stoi(drugAmount) > drugs[stoi(drugNumber)-1].getAmount()) break;

                            ph.clear();
                            ph.push_back(date);
                            ph.push_back(drugs[stoi(drugNumber) - 1].getIdentifier());
                            ph.push_back("0");
                            ph.push_back(drugAmount);
                            ph.push_back(to_string(stoi(drugAmount) * drugs[stoi(drugNumber) - 1].getPrice()));
                            clients[userId].addPurchaseHistory(ph);

                            cli.open( clientsFilePath );
                            tmp.open( tmpFilePath );
                            while( getline( cli, line ) ) {
                                if ( line.find(clients[userId].getIdentifier()) == string::npos ){
                                    tmp << line << endl;
                                } else {
                                    tmp << line << ph[0] << "," << ph[1] << "," << ph[2] << "," << ph[3] << "," << ph[4] << ";" << endl;
                                }
                            }
                            cli.close();
                            tmp.close();
                            remove("Files/clients.txt");
                            rename("Files/tmp.txt", "Files/clients.txt");

                            dru.open( drugsFilePath );
                            tmp.open( tmpFilePath );
                            while( getline( dru, line ) ) {
                                if ( line.find(drugs[stoi(drugNumber) - 1].getIdentifier()) == string::npos ){
                                    tmp << line << endl;
                                } else {
                                    toChange = line;
                                    toChange = regex_replace(toChange,
                                                             regex(";" + to_string(drugs[stoi(drugNumber) - 1].getAmount()) + ";"),
                                                             ";" + to_string(drugs[stoi(drugNumber) - 1].getAmount() - stoi(drugAmount)) + ";");
                                    tmp << toChange << endl;
                                }
                            }
                            dru.close();
                            tmp.close();
                            remove("Files/drugs.txt");
                            rename("Files/tmp.txt", "Files/drugs.txt");

                            drugs[stoi(drugNumber) - 1].setAmount( stoi(drugAmount) );
                            cout << "Drugs bought." << endl;

                            break;
                        case '2':
                            i = 0;
                            cout << endl << "Prescription drugs:" << endl;
                            for (auto &d: prescriptionDrugs) {
                                cout << ++i << ".\t id - " << d.getIdentifier() << "\t name - " << d.getName()
                                     << " description - " << d.getDescription()
                                     << " amount - " << d.getAmount() << "\t price/1 - " << d.getPrice() << endl;
                            }
                            cout << endl << "Chose drug number:" << endl;
                            cin >> drugNumber;
                            if (stoi(drugNumber) < 1 || stoi(drugNumber) > i) break;
                            cout << "Chose drug amount:" << endl;
                            cin >> drugAmount;
                            if (stoi(drugAmount) < 1 || stoi(drugAmount) > prescriptionDrugs[stoi(drugNumber)-1].getAmount()) break;

                            if(clients[userId].hasPrescription(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier())){
                                ph.clear();
                                ph.push_back(date);
                                ph.push_back(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier());
                                ph.push_back("1");
                                ph.push_back(drugAmount);
                                ph.push_back(to_string(stoi(drugAmount) * prescriptionDrugs[stoi(drugNumber) - 1].getPrice()));
                                clients[userId].addPurchaseHistory(ph);

                                cli.open( clientsFilePath );
                                tmp.open( tmpFilePath );
                                while( getline( cli, line ) ) {
                                    if ( line.find(clients[userId].getIdentifier()) == string::npos ){
                                        tmp << line << endl;
                                    } else {
                                        toChange = line;
                                        toChange = regex_replace(toChange,
                                                                 regex(";" + prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier() + ";"),
                                                                 ";-;");
                                        toChange = regex_replace(toChange,
                                                                 regex(";" + prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier() + ","),
                                                                 ";");
                                        toChange = regex_replace(toChange,
                                                                 regex("," + prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier() + ";"),
                                                                 ";");

                                        tmp << toChange << ph[0] << "," << ph[1] << "," << ph[2] << "," << ph[3] << "," << ph[4] << ";" << endl;
                                    }
                                }
                                cli.close();
                                tmp.close();
                                remove("Files/clients.txt");
                                rename("Files/tmp.txt", "Files/clients.txt");

                                dru.open( drugsFilePath );
                                tmp.open( tmpFilePath );
                                while( getline( dru, line ) ) {
                                    if ( line.find(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier()) == string::npos ){
                                        tmp << line << endl;
                                    } else {
                                        toChange = line;
                                        toChange = regex_replace(toChange,
                                                                 regex(";" + to_string(prescriptionDrugs[stoi(drugNumber) - 1].getAmount()) + ";"),
                                                                 ";" + to_string(prescriptionDrugs[stoi(drugNumber) - 1].getAmount() - stoi(drugAmount)) + ";");
                                        tmp << toChange << endl;
                                    }
                                }
                                dru.close();
                                tmp.close();
                                remove("Files/drugs.txt");
                                rename("Files/tmp.txt", "Files/drugs.txt");

                                prescriptionDrugs[stoi(drugNumber) - 1].setAmount( stoi(drugAmount) );

                                cout << "Drugs bought, prescription deleted." << endl;

                                clients[userId].removePrescription(prescriptionDrugs[stoi(drugNumber) - 1].getIdentifier());
                            } else cout << "Client has not prescription" << endl;

                            break;
                        case '3':
                            i = 0;
                            isEmpty = true;
                            cout << endl << "My prescriptions:" << endl;
                            for (auto &d: clients[userId].getPrescriptions()) {
                                for (auto &e: prescriptionDrugs) {
                                    if (e.getIdentifier() == d){
                                        isEmpty = false;
                                        cout << ++i << ".\t id - " << e.getIdentifier() << "\t name - " << e.getName()
                                             << " description - " << e.getDescription() << endl;
                                    }
                                }
                            }
                            if (isEmpty) cout << "You have not prescriptions." << endl;
                            break;
                        case '4':
                            i = 0;
                            isEmpty = true;
                            cout << endl << "My purchases history:" << endl;
                            ph.clear();
                            ph = getPurchasesHistory(clients[userId]);
                            for (int j = 0; j < ph.size()/5; j++) {
                                isEmpty = false;
                                cout << i+1 << ".\t date - " << ph[i*5] << "\t drug id - " << ph[i*5 + 1]
                                     << "\t prescription - ";
                                if (ph[i*5 + 2] == "1") cout << "true \t";
                                else cout << "false \t";
                                cout <<  " amount - " << ph[i*5 + 3] << "\t full price - " << ph[i*5 + 4]
                                     << endl;
                                i ++;
                            }
                            if (isEmpty) cout << "You have not purchases history." << endl;
                            break;
                    }
                    if (decision == '5') { validLogin = false; break; }
                }

                break;
        }
        if (decision == '3') break;
    }

    return 0;
}