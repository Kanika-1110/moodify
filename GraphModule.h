#ifndef GRAPH_MODULE_H
#define GRAPH_MODULE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "hashingmodule.h"

using namespace std;

class GraphModule {
private:
    unordered_map<int, vector<int>> graph;

public:
    void connectSongs(int song1, int song2);

    vector<int> getBFSPlaylist(
        int startingSong,
        const string &mood,
        const HashingModule &library
    );

    void displayPlaylist(
        const vector<int> &playlist,
        const HashingModule &library
    );
};

#endif