#include <iostream>
#include "hashingmodule.h"
#include "recommendation.h"

using namespace std;

int main() {
    HashingModule library;

    library.addSong(1, "Blinding Lights",
                    "The Weeknd", "Energetic");

    library.addSong(2, "Levitating",
                    "Dua Lipa", "Energetic");

    library.addSong(3, "Shape of You",
                    "Ed Sheeran", "Happy");

    library.addSong(4, "Perfect",
                    "Ed Sheeran", "Romantic");

    library.addSong(5, "Someone Like You",
                    "Adele", "Sad");

    Recommendation recommendation(library);

    recommendation.run();

    return 0;
}