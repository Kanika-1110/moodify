#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

// ----------------------------------------------------
// SONG STRUCTURE
// ----------------------------------------------------
struct Song {
    int id;
    string title;
    string artist;
    string mood;
};

// ----------------------------------------------------
// MOODIFY CLASS
// ----------------------------------------------------
class Moodify {

private:

    // Hash Table:
    // song ID -> Song
    unordered_map<int, Song> songTable;

    // Graph:
    // song ID -> list of connected songs
    unordered_map<int, vector<int>> graph;

public:

    // ------------------------------------------------
    // ADD SONG
    // ------------------------------------------------
    void addSong(int id, string title, string artist, string mood) {

        Song s;

        s.id = id;
        s.title = title;
        s.artist = artist;
        s.mood = mood;

        // Store song in hash table
        songTable[id] = s;
    }

    // ------------------------------------------------
    // CONNECT TWO SONGS
    // ------------------------------------------------
    void connectSongs(int song1, int song2) {

        graph[song1].push_back(song2);
        graph[song2].push_back(song1);
    }

    // ------------------------------------------------
    // DISPLAY ALL SONGS
    // ------------------------------------------------
    void displaySongs() {

        cout << "\n========== MOODIFY SONG DATABASE ==========\n";

        for (auto &entry : songTable) {

            Song s = entry.second;

            cout << "ID: " << s.id
                 << " | " << s.title
                 << " | " << s.artist
                 << " | Mood: " << s.mood
                 << endl;
        }
    }

    // ------------------------------------------------
    // FIND SONGS BASED ON MOOD
    // ------------------------------------------------
    vector<int> getMoodSongs(string mood) {

        vector<int> result;

        for (auto &entry : songTable) {

            Song s = entry.second;

            if (s.mood == mood) {
                result.push_back(s.id);
            }
        }

        return result;
    }

    // ------------------------------------------------
    // BFS RECOMMENDATION
    // ------------------------------------------------
    void BFS(int startingSong, int maxRecommendations) {

        queue<int> q;
        unordered_map<int, bool> visited;

        q.push(startingSong);
        visited[startingSong] = true;

        int count = 0;

        cout << "\n========== RECOMMENDED SONGS ==========\n";

        while (!q.empty() && count < maxRecommendations) {

            int current = q.front();
            q.pop();

            Song s = songTable[current];

            cout << count + 1 << ". "
                 << s.title
                 << " - "
                 << s.artist
                 << endl;

            count++;

            // Visit connected songs
            for (int neighbour : graph[current]) {

                if (!visited[neighbour]) {

                    visited[neighbour] = true;
                    q.push(neighbour);
                }
            }
        }
    }

    // ------------------------------------------------
    // GENERATE RECOMMENDATIONS
    // ------------------------------------------------
    void recommend(string mood) {

        vector<int> moodSongs = getMoodSongs(mood);

        if (moodSongs.empty()) {

            cout << "\nNo songs found for this mood.\n";
            return;
        }

        cout << "\nMood selected: " << mood << endl;

        cout << "\nSongs matching your mood:\n";

        for (int id : moodSongs) {

            Song s = songTable[id];

            cout << "- "
                 << s.title
                 << " by "
                 << s.artist
                 << endl;
        }

        // Start BFS from first matching song
        cout << "\nGenerating related recommendations...\n";

        BFS(moodSongs[0], 5);
    }
};


// ====================================================
// MAIN FUNCTION
// ====================================================

int main() {

    Moodify moodify;

    // ------------------------------------------------
    // SAMPLE SONG DATABASE
    // ------------------------------------------------

    moodify.addSong(
        1,
        "Blinding Lights",
        "The Weeknd",
        "Energetic"
    );

    moodify.addSong(
        2,
        "Levitating",
        "Dua Lipa",
        "Energetic"
    );

    moodify.addSong(
        3,
        "Shape of You",
        "Ed Sheeran",
        "Happy"
    );

    moodify.addSong(
        4,
        "Perfect",
        "Ed Sheeran",
        "Romantic"
    );

    moodify.addSong(
        5,
        "Someone Like You",
        "Adele",
        "Sad"
    );

    moodify.addSong(
        6,
        "Stay",
        "The Kid LAROI",
        "Sad"
    );

    moodify.addSong(
        7,
        "Believer",
        "Imagine Dragons",
        "Energetic"
    );

    moodify.addSong(
        8,
        "Until I Found You",
        "Stephen Sanchez",
        "Romantic"
    );


    // ------------------------------------------------
    // CREATE SONG RELATIONSHIPS
    // ------------------------------------------------

    moodify.connectSongs(1, 2);
    moodify.connectSongs(1, 7);

    moodify.connectSongs(2, 3);
    moodify.connectSongs(3, 4);

    moodify.connectSongs(4, 8);

    moodify.connectSongs(5, 6);
    moodify.connectSongs(6, 4);


    // ------------------------------------------------
    // USER INTERFACE
    // ------------------------------------------------

    cout << "============================================\n";
    cout << "              MOODIFY\n";
    cout << "   Personalized Music Recommendation\n";
    cout << "============================================\n";

    cout << "\nAvailable moods:\n";
    cout << "1. Happy\n";
    cout << "2. Sad\n";
    cout << "3. Romantic\n";
    cout << "4. Energetic\n";

    int choice;

    cout << "\nEnter your choice: ";
    cin >> choice;

    string selectedMood;

    switch (choice) {

        case 1:
            selectedMood = "Happy";
            break;

        case 2:
            selectedMood = "Sad";
            break;

        case 3:
            selectedMood = "Romantic";
            break;

        case 4:
            selectedMood = "Energetic";
            break;

        default:
            cout << "\nInvalid choice.\n";
            return 0;
    }

    // ------------------------------------------------
    // GENERATE RECOMMENDATIONS
    // ------------------------------------------------

    moodify.recommend(selectedMood);

    cout << "\n============================================\n";
    cout << "        End of Phase 2 Prototype\n";
    cout << "============================================\n";

    return 0;
}