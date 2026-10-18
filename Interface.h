#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "hashingmodule.h"
#include "Recommendation.h"

using namespace std;

class Interface {
private:
    HashingModule &library;
    Recommendation recommendation;
    string username;

    bool login();
    void displayHeader();
    void displayMenu();

public:
    explicit Interface(HashingModule &lib);
    void run();
};

#endif