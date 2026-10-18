#include "HashingModule.h"

int main() {
    HashingModule library;

    library.addSong(1, "Blinding Lights",
                    "The Weeknd", "Energetic");

    library.addSong(2, "Levitating",
                    "Dua Lipa", "Energetic");

    library.addSong(3, "Perfect",
                    "Ed Sheeran", "Romantic");

    library.displaySongs();

    cout << "\nEnergetic song IDs:\n";

    vector<int> songs = library.getMoodSongs("Energetic");

    for (int id : songs) {
        Song s;

        if (library.getSongById(id, s)) {
            cout << s.id << " - " << s.title << "\n";
        }
    }

    cout << "\nTotal songs: " << library.getSongCount() << "\n";

    return 0;
}