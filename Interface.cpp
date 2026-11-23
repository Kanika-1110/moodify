#include "Interface.h"
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

Interface::Interface(HashingModule &lib, GraphModule &graph)
    : library(lib), recommendation(lib, graph) {}


bool Interface::login() {
    int choice;
    string password;

    cout << "\n========================================\n";
    cout << "          WELCOME TO MOODIFY\n";
    cout << "       MUSIC THAT MATCHES YOU\n";
    cout << "========================================\n";

    cout << "1. Login\n";
    cout << "2. Create a guest session\n";
    cout << "Enter choice: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    if (choice == 1) {
        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        if (username == "music" && password == "1234") {
            cout << "Login successful. Welcome, "
                 << username << "!\n";
            return true;
        }

        cout << "Invalid username or password.\n";
        return false;
    }

    if (choice == 2) {
        cout << "Enter your display name: ";
        cin >> username;

        cout << "Guest session created!\n";
        return true;
    }

    cout << "Invalid choice.\n";
    return false;
}

void Interface::displayHeader() {
    cout << "\n========================================\n";
    cout << "          MOODIFY MUSIC STUDIO\n";
    cout << "          YOUR MOOD. YOUR MUSIC.\n";
    cout << "========================================\n";
}

void Interface::displayMenu() {
    cout << "\n1. Discover music by mood\n";
    cout << "2. Explore the music library\n";
    cout << "3. Choose another mood\n";
    cout << "4. Like a song\n";
    cout << "5. Dislike a song\n";
    cout << "6. View liked songs\n";
    cout << "7. View disliked songs\n";
    cout << "8. Exit Moodify\n";
    cout << "Enter your choice: ";
}

void Interface::likeSong() {
    if (library.getSongCount() == 0) {
        cout << "The music library is empty.\n";
        return;
    }

    library.displaySongs();
    cout << "\nEnter the ID of the song to like: ";

    int songID;
    if (!(cin >> songID)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid song ID.\n";
        return;
    }

    Song song;
    if (!library.getSongById(songID, song)) {
        cout << "No song has that ID.\n";
        return;
    }

    auto liked = find(likedSongIDs.begin(), likedSongIDs.end(), songID);
    if (liked != likedSongIDs.end()) {
        cout << "\"" << song.title << "\" is already in your liked songs.\n";
        return;
    }

    dislikedSongIDs.erase(
        remove(dislikedSongIDs.begin(), dislikedSongIDs.end(), songID),
        dislikedSongIDs.end()
    );
    likedSongIDs.push_back(songID);
    cout << "Liked \"" << song.title << "\" by "
         << song.artist << ".\n";
}

void Interface::dislikeSong() {
    if (library.getSongCount() == 0) {
        cout << "The music library is empty.\n";
        return;
    }
    library.displaySongs();
    cout << "\nEnter the ID of the song to dislike: ";

    int songID;
    if (!(cin >> songID)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid song ID.\n";
        return;
    }

    Song song;
    if (!library.getSongById(songID, song)) {
        cout << "No song has that ID.\n";
        return;
    }

    auto disliked = find(dislikedSongIDs.begin(), dislikedSongIDs.end(), songID);
    if (disliked != dislikedSongIDs.end()) {
        cout << "\"" << song.title << "\" is already marked as disliked.\n";
        return;
    }

    likedSongIDs.erase(
        remove(likedSongIDs.begin(), likedSongIDs.end(), songID),
        likedSongIDs.end()
    );
    dislikedSongIDs.push_back(songID);
    cout << "Marked \"" << song.title << "\" by "
         << song.artist << " as disliked.\n";
}

void Interface::displayLikedSongs() const {
    cout << "\n========== YOUR LIKED SONGS ==========\n";
    if (likedSongIDs.empty()) {
        cout << "You haven't liked any songs yet.\n";
        return;
    }

    for (int songID : likedSongIDs) {
        Song song;
        if (library.getSongById(songID, song)) {
            cout << "\nID: " << song.id
                 << "\nTitle: " << song.title
                 << "\nArtist: " << song.artist
                 << "\nMood: " << song.mood << "\n";
        }
    }
}

void Interface::displayDislikedSongs() const {
    cout << "\n========== YOUR DISLIKED SONGS ==========\n";
    if (dislikedSongIDs.empty()) {
        cout << "You haven't disliked any songs yet.\n";
        return;
    }

    for (int songID : dislikedSongIDs) {
        Song song;
        if (library.getSongById(songID, song)) {
            cout << "\nID: " << song.id
                 << "\nTitle: " << song.title
                 << "\nArtist: " << song.artist
                 << "\nMood: " << song.mood << "\n";
        }
    }
}

void Interface::run() {
    if (!login()) {
        cout << "Login failed. Restart the application to try again.\n";
        return;
    }

    int choice = 0;

    do {
        displayHeader();
        cout << "Hello, " << username
             << "! Ready to find your vibe?\n";

        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1:
            case 3:
                recommendation.run();
                break;

            case 2:
                library.displaySongs();
                break;

            case 4:
                likeSong();
                break;

            case 5:
                dislikeSong();
                break;

            case 6:
                displayLikedSongs();
                break;

            case 7:
                displayDislikedSongs();
                break;

            case 8:
                cout << "Thanks for listening with Moodify, "
                     << username << "!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 8);
}
