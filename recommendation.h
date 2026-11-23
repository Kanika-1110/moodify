#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <iostream>
#include <string>
#include <vector>
#include "GraphModule.h"

using namespace std;

class Recommendation {
private:
    HashingModule &library;
    GraphModule &graphModule;

public:
   
    Recommendation(HashingModule &lib, GraphModule &graph)
        : library(lib), graphModule(graph) {}

   
    string selectMood() {
        int choice;

        cout << "\n========== HOW ARE YOU FEELING? ==========\n";
        cout << "1. Happy\n";
        cout << "2. Sad\n";
        cout << "3. Romantic\n";
        cout << "4. Chill\n";
        cout << "5. Focused\n";
        cout << "6. Melancholy\n";
        cout << "7. Energetic\n";
        cout << "Select your mood (1-7): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            return "";
        }

        switch (choice) {
            case 1: return "Happy";
            case 2: return "Sad";
            case 3: return "Romantic";
            case 4: return "Chill";
            case 5: return "Focused";
            case 6: return "Melancholy";
            case 7: return "Energetic";
            default:
                cout << "Invalid mood selection!\n";
                return "";
        }
    }

   
    void recommend(const string &mood) {
        vector<int> songIDs = library.getMoodSongs(mood);

        cout << "\n========== MOODIFY RECOMMENDATIONS ==========\n";
        cout << "Current mood: " << mood << "\n";

        if (songIDs.empty()) {
            cout << "Sorry! No songs found for this mood.\n";
            return;
        }

        vector<int> playlist =
            graphModule.getBFSPlaylist(songIDs.front(), mood, library);
        graphModule.displayPlaylist(playlist, library);
    }

   
    void run() {
        string mood = selectMood();

        if (!mood.empty()) {
            recommend(mood);
        }
    }
};

#endif
