#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include "HashingModule.h"
#include "GraphModule.h"
#include "recommendation.h"

using namespace std;

class Interface {
private:
    HashingModule &library;
    Recommendation recommendation;
    string username;
    vector<int> likedSongIDs;
    vector<int> dislikedSongIDs;

    bool login();
    void displayHeader();
    void displayMenu();
    void likeSong();
    void dislikeSong();
    void displayLikedSongs() const;
    void displayDislikedSongs() const;

public:
    Interface(HashingModule &lib, GraphModule &graph);
    void run();
};

#endif
