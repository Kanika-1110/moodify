#ifndef HASHING_MODULE_H
#define HASHING_MODULE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;


struct Song {
    int id;
    string title;
    string artist;
    string mood;
};

class HashingModule {
private:
    
    unordered_map<int, Song> songTable;

public:
    bool addSong(int id, string title,
                 string artist, string mood) {

      
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

    bool hasSong(int id) const {
        return songTable.find(id) != songTable.end();
    }

    bool hasMood(const Song &song, const string &mood) const {
        size_t start = 0;

        while (start <= song.mood.size()) {
            size_t end = song.mood.find(", ", start);
            if (song.mood.substr(start, end - start) == mood) {
                return true;
            }

            if (end == string::npos) {
                break;
            }
            start = end + 2;
        }

        return false;
    }


    bool getSongById(int id, Song &result) const {
        auto it = songTable.find(id);

        if (it == songTable.end()) {
            return false;
        }

        result = it->second;
        return true;
    }

    
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

  
    vector<int> getMoodSongs(const string &mood) const {
        vector<int> result;

        for (const auto &entry : songTable) {
            const Song &s = entry.second;

            if (hasMood(s, mood)) {
                result.push_back(s.id);
            }
        }

        sort(result.begin(), result.end());
        return result;
    }

   
    size_t getSongCount() const {
        return songTable.size();
    }
};

#endif
