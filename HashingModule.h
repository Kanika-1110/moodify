#ifndef HASHING_MODULE_H
#define HASHING_MODULE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Stores information about each song
struct Song {
    int id;
    string title;
    string artist;
    string mood;
};

class HashingModule {
private:
    // Hash table: song ID -> Song details
    unordered_map<int, Song> songTable;

public:
    // Add a song to the music library
    bool addSong(int id, string title,
                 string artist, string mood) {

        // Prevent duplicate song IDs
        if (songTable.find(id) != songTable.end()) {
            cout << "Song ID already exists!\n";
            return false;
        }

        Song s;
        s.id = id;
        s.title = title;
        s.artist = artist;
        s.mood = mood;

        songTable[id] = s;
        return true;
    }

    // Check whether a song ID exists
    bool hasSong(int id) const {
        return songTable.find(id) != songTable.end();
    }

    // Retrieve a song using its ID
    bool getSongById(int id, Song &result) const {
        auto it = songTable.find(id);

        if (it == songTable.end()) {
            return false;
        }

        result = it->second;
        return true;
    }

    // Display all songs in sorted ID order
    void displaySongs() const {
        vector<int> ids;

        for (const auto &entry : songTable) {
            ids.push_back(entry.first);
        }

        sort(ids.begin(), ids.end());

        cout << "\n========== MOODIFY MUSIC LIBRARY ==========\n";

        if (ids.empty()) {
            cout << "The music library is empty.\n";
            return;
        }

        for (int id : ids) {
            const Song &s = songTable.at(id);

            cout << "\nID: " << s.id
                 << "\nTitle: " << s.title
                 << "\nArtist: " << s.artist
                 << "\nMood: " << s.mood << "\n";
        }
    }

    // Return song IDs matching the selected mood
    vector<int> getMoodSongs(const string &mood) const {
        vector<int> result;

        for (const auto &entry : songTable) {
            const Song &s = entry.second;

            if (s.mood == mood) {
                result.push_back(s.id);
            }
        }

        sort(result.begin(), result.end());
        return result;
    }

    // Return the total number of songs
    size_t getSongCount() const {
        return songTable.size();
    }
};

#endif