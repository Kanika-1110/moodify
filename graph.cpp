#include "GraphModule.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Connect two related songs (undirected graph)
void GraphModule::connectSongs(int song1, int song2) {
    auto &list1 = graph[song1];
    auto &list2 = graph[song2];

    if (find(list1.begin(), list1.end(), song2) == list1.end())
        list1.push_back(song2);

    if (find(list2.begin(), list2.end(), song1) == list2.end())
        list2.push_back(song1);
}

// Generate a playlist using BFS
vector<int> GraphModule::getBFSPlaylist(
    int startingSong,
    const string &mood,
    const HashingModule &library
) {
    vector<int> playlist;
    queue<int> q;
    unordered_set<int> visited;

    Song start;

    if (!library.getSongById(startingSong, start) ||
        start.mood != mood) {
        return playlist;
    }

    q.push(startingSong);
    visited.insert(startingSong);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        Song song;

        if (!library.getSongById(current, song))
            continue;

        if (song.mood != mood)
            continue;

        playlist.push_back(current);

        auto it = graph.find(current);

        if (it != graph.end()) {
            for (int neighbour : it->second) {
                if (visited.insert(neighbour).second)
                    q.push(neighbour);
            }
        }
    }

    // Include other songs with the same mood even if disconnected
    vector<int> remaining = library.getMoodSongs(mood);

    for (int id : remaining) {
        if (visited.insert(id).second)
            playlist.push_back(id);
    }

    return playlist;
}

// Display the generated playlist
void GraphModule::displayPlaylist(
    const vector<int> &playlist,
    const HashingModule &library
) {
    cout << "\n========== BFS PERSONALIZED PLAYLIST ==========\n";

    if (playlist.empty()) {
        cout << "No recommendations available.\n";
        return;
    }

    int count = 1;

    for (int id : playlist) {
        Song song;

        if (library.getSongById(id, song)) {
            cout << count++ << ". " << song.title
                 << "\n   Artist: " << song.artist
                 << "\n   Mood: " << song.mood << "\n\n";
        }
    }
}