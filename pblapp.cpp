#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <filesystem>
#include <algorithm>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

// For MSVC: uncomment if needed
// #pragma comment(lib, "winmm.lib")

struct Song {
    string title;
    string artist;
    string path;
};

class MusicPlayer {
private:
    vector<Song> library;
    int currentIndex = -1;

    bool runMciCommand(const string& command) {
        return mciSendStringA(command.c_str(), NULL, 0, NULL) == 0;
    }

    string makeAliasName(int index) {
        return "song" + to_string(index);
    }

public:
    void addSong(const string& title, const string& artist, const string& path) {
        library.push_back({title, artist, path});
        cout << "Added: " << title << " - " << artist << "\n";
    }

    void showLibrary() {
        if (library.empty()) {
            cout << "\nNo songs in library.\n";
            return;
        }

        cout << "\n=== MUSIC LIBRARY ===\n";
        for (size_t i = 0; i < library.size(); ++i) {
            cout << i + 1 << ". " << library[i].title
                 << " - " << library[i].artist << "\n";
        }
    }

    bool playSongByPath(const string& filePath) {
        if (filePath.empty()) {
            cout << "Path is empty.\n";
            return false;
        }

        std::filesystem::path path(filePath);
        if (!std::filesystem::exists(path)) {
            cout << "File not found: " << filePath << "\n";
            return false;
        }

        string extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });

        string mediaType = "";
        if (extension == ".MP3" || extension == ".MPG" || extension == ".MPEG" || extension == ".M4A" || extension == ".AAC") {
            mediaType = "mpegvideo";
        } else if (extension == ".WAV" || extension == ".WAVE") {
            mediaType = "waveaudio";
        } else {
            cout << "Unsupported file type: " << filePath << "\n";
            cout << "Please use a valid .mp3 or .wav file.\n";
            return false;
        }

        runMciCommand("close media");

        string openCmd = "open \"" + filePath + "\" type " + mediaType + " alias media";
        if (runMciCommand(openCmd)) {
            if (runMciCommand("play media")) {
                cout << "Playing: " << filePath << "\n";
                return true;
            }
            cout << "File opened, but playback command failed.\n";
            return false;
        }

        cout << "Failed to open file: " << filePath << "\n";
        cout << "Please check that the file path is valid and the format is supported.\n";
        return false;
    }

    bool playSong(int index) {
        if (index < 0 || index >= (int)library.size()) {
            cout << "Invalid song number.\n";
            return false;
        }

        currentIndex = index;
        cout << "\nNow playing: " << library[index].title
             << " - " << library[index].artist << "\n";

        return playSongByPath(library[index].path);
    }

    void pauseSong() {
        if (runMciCommand("pause media")) {
            cout << "\nSong paused.\n";
        } else {
            cout << "\nNo song is playing.\n";
        }
    }

    void resumeSong() {
        if (runMciCommand("resume media")) {
            cout << "\nSong resumed.\n";
        } else {
            cout << "\nNo paused song to resume.\n";
        }
    }

    void stopSong() {
        if (runMciCommand("stop media")) {
            cout << "\nSong stopped.\n";
        } else {
            cout << "\nNo song is playing.\n";
        }

        runMciCommand("close media");
    }

    void playRealFilePrompt() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string filePath;
        cout << "\nEnter full path to a .mp3 or .wav file: ";
        getline(cin, filePath);

        if (playSongByPath(filePath)) {
            cout << "Playing: " << filePath << "\n";
        }
    }

    void menu() {
        int choice = -1;

        while (choice != 0) {
            cout << "\n=========================\n";
            cout << "   MUSIC PLAYER MENU\n";
            cout << "=========================\n";
            cout << "1. Add song\n";
            cout << "2. Show library\n";
            cout << "3. Play song\n";
            cout << "4. Pause\n";
            cout << "5. Resume\n";
            cout << "6. Stop\n";
            cout << "7. Play a real MP3/WAV file\n";
            cout << "0. Exit\n";
            cout << "Choose: ";

            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid number.\n";
                continue;
            }

            switch (choice) {
                case 1: {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    string title, artist, path;
                    cout << "Song title: ";
                    getline(cin, title);

                    cout << "Artist: ";
                    getline(cin, artist);

                    cout << "File path: ";
                    getline(cin, path);

                    addSong(title, artist, path);
                    break;
                }

                case 2:
                    showLibrary();
                    break;

                case 3: {
                    if (library.empty()) {
                        cout << "Library is empty. Add a song first.\n";
                        break;
                    }

                    showLibrary();
                    int songNum;
                    cout << "Enter song number: ";
                    cin >> songNum;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a valid song number.\n";
                        break;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    playSong(songNum - 1);
                    break;
                }

                case 4:
                    pauseSong();
                    break;

                case 5:
                    resumeSong();
                    break;

                case 6:
                    stopSong();
                    break;

                case 7:
                    playRealFilePrompt();
                    break;

                case 0:
                    stopSong();
                    cout << "Goodbye!\n";
                    break;

                default:
                    cout << "Invalid choice.\n";
                    break;
            }
        }
    }
};

int main() {
    MusicPlayer player;
    player.menu();
    return 0;
}