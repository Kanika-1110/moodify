#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <iostream>
#include <string>
#include <vector>
#include "hashingmodule.h"

using namespace std;

class Recommendation {
private:
    HashingModule &library;

public:
    // Use the existing music library
    explicit Recommendation(HashingModule &lib)
        : library(lib) {}

    // Let the user select a mood
    string selectMood() {
        int choice;

        cout << "\n========== HOW ARE YOU FEELING? ==========\n";
        cout << "1. Happy\n";
        cout << "2. Sad\n";
        cout << "3. Romantic\n";
        cout << "4. Energetic\n";
        cout << "Select your mood (1-4): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            return "";
        }

        switch (choice) {
            case 1: return "Happy";
            case 2: return "Sad";
            case 3: return "Romantic";
            case 4: return "Energetic";
            default:
                cout << "Invalid mood selection!\n";
                return "";
        }
    }

    // Generate recommendations for the selected mood
    void recommend(const string &mood) {
        vector<int> songIDs = library.getMoodSongs(mood);

        cout << "\n========== MOODIFY RECOMMENDATIONS ==========\n";
        cout << "Current mood: " << mood << "\n";

        if (songIDs.empty()) {
            cout << "Sorry! No songs found for this mood.\n";
            return;
        }

        cout << "\nYour personalized playlist:\n\n";

        int count = 1;

        for (int id : songIDs) {
            Song song;

            if (library.getSongById(id, song)) {
                cout << count++ << ". " << song.title << "\n";
                cout << "   Artist: " << song.artist << "\n";
                cout << "   Mood: " << song.mood << "\n\n";
            }
        }
    }

    // Complete mood-selection and recommendation flow
    void run() {
        string mood = selectMood();

        if (!mood.empty()) {
            recommend(mood);
        }
    }
};

#endif