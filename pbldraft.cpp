#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

const string RESET = "\033[0m";
const string PINK = "\033[38;5;213m";
const string PURPLE = "\033[38;5;141m";
const string LAVENDER = "\033[38;5;183m";

// ======================================================
//                    SONG STRUCTURE
// ======================================================

struct Song {
    int id;
    string title;
    string artist;
    string mood;

    int playCount;
    int likeCount;
    int skipCount;

    vector<int> relatedSongs;
};

// ======================================================
//               DOUBLY LINKED LIST PLAYLIST
// ======================================================

struct PlaylistNode {
    int songId;
    PlaylistNode* next;
    PlaylistNode* prev;

    PlaylistNode(int id) {
        songId = id;
        next = nullptr;
        prev = nullptr;
    }
};

class Playlist {
private:
    PlaylistNode* head;
    PlaylistNode* tail;
    PlaylistNode* current;

public:
    Playlist() {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    ~Playlist() {
        clear();
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void addSong(int songId) {
        PlaylistNode* newNode = new PlaylistNode(songId);

        if (head == nullptr) {
            head = tail = current = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void clear() {
        PlaylistNode* temp = head;

        while (temp != nullptr) {
            PlaylistNode* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    int getCurrentSong() {
        if (current == nullptr) {
            return -1;
        }

        return current->songId;
    }

    bool nextSong() {
        if (current != nullptr && current->next != nullptr) {
            current = current->next;
            return true;
        }

        return false;
    }

    bool previousSong() {
        if (current != nullptr && current->prev != nullptr) {
            current = current->prev;
            return true;
        }

        return false;
    }

    void display(unordered_map<int, Song>& songs) {
        if (head == nullptr) {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        PlaylistNode* temp = head;
        int number = 1;

        cout << "\n========== CURRENT PLAYLIST ==========\n";

        while (temp != nullptr) {
            Song& song = songs[temp->songId];

            if (temp == current) {
                cout << ">> ";
            } else {
                cout << "   ";
            }

            cout << number << ". "
                 << song.title
                 << " - "
                 << song.artist
                 << " [" << song.mood << "]\n";

            temp = temp->next;
            number++;
        }
    }

    vector<int> getAllSongIds() {
        vector<int> ids;
        PlaylistNode* temp = head;

        while (temp != nullptr) {
            ids.push_back(temp->songId);
            temp = temp->next;
        }

        return ids;
    }
};

// ======================================================
//                     MUSIC SYSTEM
// ======================================================

class Moodify {
private:

    // Main Song Database
    unordered_map<int, Song> songs;

    // Mood -> List of Song IDs
    unordered_map<string, vector<int>> moodSongs;

    // Artist -> Number of times user listened
    unordered_map<string, int> artistFrequency;

    // Mood -> Number of times user selected/listened
    unordered_map<string, int> moodFrequency;

    // Recently played songs
    stack<int> recentlyPlayed;

    // Songs waiting to be played
    queue<int> playQueue;

    // Personalized playlist
    Playlist playlist;

    // Prevent recently played songs from dominating
    vector<int> recentHistory;

    // ==================================================
    //               ADD SAMPLE SONG
    // ==================================================

    void addSong(
        int id,
        string title,
        string artist,
        string mood,
        vector<int> related = {}
    ) {
        Song newSong;

        newSong.id = id;
        newSong.title = title;
        newSong.artist = artist;
        newSong.mood = mood;
        newSong.playCount = 0;
        newSong.likeCount = 0;
        newSong.skipCount = 0;
        newSong.relatedSongs = related;

        songs[id] = newSong;

        // Add song ID to its mood category
        moodSongs[mood].push_back(id);
    }

    // ==================================================
    //               LOAD SONG DATABASE
    // ==================================================

    void loadSampleSongs() {

        // CALM SONGS
        addSong(1, "Husn", "Anuv Jain", "Calm", {2, 3});
        addSong(2, "Baarishein", "Anuv Jain", "Calm", {1, 4});
        addSong(3, "Alag Aasmaan", "Anuv Jain", "Calm", {1, 2});
        addSong(4, "Iktara", "Kavita Seth", "Calm", {2, 5});
        addSong(5, "Apocalypse", "Cigarettes After Sex", "Calm", {4});

        // SAD SONGS
        addSong(6, "Agar Tum Saath Ho", "Arijit Singh", "Sad", {7, 8});
        addSong(7, "Choo Lo", "The Local Train", "Sad", {6, 9});
        addSong(8, "The Night We Met", "Lord Huron", "Sad", {6});
        addSong(9, "Tune Jo Na Kaha", "Mohit Chauhan", "Sad", {7});

        // HAPPY SONGS
        addSong(10, "Happy", "Pharrell Williams", "Happy", {11});
        addSong(11, "Ilahi", "Arijit Singh", "Happy", {10, 12});
        addSong(12, "Love You Zindagi", "Amit Trivedi", "Happy", {11});

        // ROMANTIC SONGS
        addSong(13, "Kesariya", "Arijit Singh", "Romantic", {14, 15});
        addSong(14, "Heeriye", "Jasleen Royal", "Romantic", {13});
        addSong(15, "Until I Found You", "Stephen Sanchez", "Romantic", {13});

        // ENERGETIC SONGS
        addSong(16, "Starboy", "The Weeknd", "Energetic", {17});
        addSong(17, "Blinding Lights", "The Weeknd", "Energetic", {16, 18});
        addSong(18, "Believer", "Imagine Dragons", "Energetic", {17});

        // FOCUS SONGS
        addSong(19, "Snowfall", "Oneheart", "Focus", {20});
        addSong(20, "Experience", "Ludovico Einaudi", "Focus", {19});
    }

    // ==================================================
    //        CHECK IF SONG WAS PLAYED RECENTLY
    // ==================================================

    bool wasRecentlyPlayed(int songId) {

        for (int id : recentHistory) {
            if (id == songId) {
                return true;
            }
        }

        return false;
    }

    // ==================================================
    //              UPDATE RECENT HISTORY
    // ==================================================

    void updateRecentHistory(int songId) {

        recentHistory.push_back(songId);

        // Keep only last 5 songs
        if (recentHistory.size() > 5) {
            recentHistory.erase(recentHistory.begin());
        }
    }

    // ==================================================
    //          CALCULATE RECOMMENDATION SCORE
    // ==================================================

    int calculateScore(int songId, string selectedMood) {

        Song& song = songs[songId];

        int score = 0;

        // 1. Mood match
        if (song.mood == selectedMood) {
            score += 40;
        }

        // 2. Artist preference
        score += artistFrequency[song.artist] * 5;

        // 3. User previously liked this song
        score += song.likeCount * 10;

        // 4. Previous plays
        score += song.playCount * 2;

        // 5. Skipping penalty
        score -= song.skipCount * 8;

        // 6. Recently played penalty
        if (wasRecentlyPlayed(songId)) {
            score -= 30;
        }

        return score;
    }

public:

    Moodify() {
        loadSampleSongs();
    }

    // ==================================================
    //                DISPLAY ALL SONGS
    // ==================================================

    void displayAllSongs() {

        cout << "\n================ SONG DATABASE ================\n";

        for (auto& pair : songs) {

            Song& song = pair.second;

            cout << song.id << ". "
                 << song.title
                 << " - " << song.artist
                 << " | Mood: " << song.mood
                 << "\n";
        }
    }

    // ==================================================
    //                  SEARCH SONG
    // ==================================================

    void searchSong() {

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string keyword;

        cout << "\nEnter song title or artist: ";
        getline(cin, keyword);

        transform(
            keyword.begin(),
            keyword.end(),
            keyword.begin(),
            ::tolower
        );

        bool found = false;

        cout << "\nSearch Results:\n";

        for (auto& pair : songs) {

            Song& song = pair.second;

            string title = song.title;
            string artist = song.artist;

            transform(
                title.begin(),
                title.end(),
                title.begin(),
                ::tolower
            );

            transform(
                artist.begin(),
                artist.end(),
                artist.begin(),
                ::tolower
            );

            if (
                title.find(keyword) != string::npos ||
                artist.find(keyword) != string::npos
            ) {
                cout << song.id << ". "
                     << song.title
                     << " - "
                     << song.artist
                     << "\n";

                found = true;
            }
        }

        if (!found) {
            cout << "No songs found.\n";
        }
    }

    // ==================================================
    //            GENERATE RECOMMENDATIONS
    // ==================================================

    void generateRecommendations() {

        cout << "\nSelect your mood:\n";
        cout << "1. Happy\n";
        cout << "2. Sad\n";
        cout << "3. Calm\n";
        cout << "4. Energetic\n";
        cout << "5. Romantic\n";
        cout << "6. Focus\n";

        int choice;

        cout << "\nEnter choice: ";
        cin >> choice;

        string selectedMood;

        switch (choice) {
            case 1: selectedMood = "Happy"; break;
            case 2: selectedMood = "Sad"; break;
            case 3: selectedMood = "Calm"; break;
            case 4: selectedMood = "Energetic"; break;
            case 5: selectedMood = "Romantic"; break;
            case 6: selectedMood = "Focus"; break;

            default:
                cout << "Invalid choice.\n";
                return;
        }

        moodFrequency[selectedMood]++;

        // Priority Queue
        // pair<score, songId>
        priority_queue<pair<int, int>> rankedSongs;

        // Get all songs for selected mood
        for (int songId : moodSongs[selectedMood]) {

            int score = calculateScore(
                songId,
                selectedMood
            );

            rankedSongs.push({
                score,
                songId
            });
        }

        // Clear old playlist
        playlist.clear();

        // Clear old queue
        while (!playQueue.empty()) {
            playQueue.pop();
        }

        cout << "\n==========================================\n";
        cout << " PERSONALIZED " << selectedMood << " PLAYLIST\n";
        cout << "==========================================\n";

        int count = 0;

        // Get top 5 recommendations
        while (!rankedSongs.empty() && count < 5) {

            int score = rankedSongs.top().first;
            int songId = rankedSongs.top().second;

            rankedSongs.pop();

            Song& song = songs[songId];

            cout << count + 1 << ". "
                 << song.title
                 << " - "
                 << song.artist
                 << " | Score: "
                 << score
                 << "\n";

            // Add to doubly linked list playlist
            playlist.addSong(songId);

            // Add to play queue
            playQueue.push(songId);

            count++;
        }

        cout << "\nPlaylist generated successfully!\n";
    }

    // ==================================================
    //                    PLAY SONG
    // ==================================================

    void playCurrentSong() {

        int songId = playlist.getCurrentSong();

        if (songId == -1) {
            cout << "\nNo playlist available.\n";
            return;
        }

        Song& song = songs[songId];

        cout << "\n====================================\n";
        cout << "NOW PLAYING\n";
        cout << song.title << " - " << song.artist << "\n";
        cout << "Mood: " << song.mood << "\n";
        cout << "====================================\n";

        // Update statistics
        song.playCount++;

        // Update artist preference
        artistFrequency[song.artist]++;

        // Update mood preference
        moodFrequency[song.mood]++;

        // Push into recently played stack
        recentlyPlayed.push(songId);

        // Update recent history
        updateRecentHistory(songId);
    }

    // ==================================================
    //                   NEXT SONG
    // ==================================================

    void nextSong() {

        if (playlist.nextSong()) {

            // Remove currently completed song from queue
            if (!playQueue.empty()) {
                playQueue.pop();
            }

            cout << "\nMoved to next song.\n";

            playCurrentSong();

        } else {

            cout << "\nYou are already at the last song.\n";
        }
    }

    // ==================================================
    //                 PREVIOUS SONG
    // ==================================================

    void previousSong() {

        if (playlist.previousSong()) {

            cout << "\nMoved to previous song.\n";

            playCurrentSong();

        } else {

            cout << "\nYou are already at the first song.\n";
        }
    }

    // ==================================================
    //                    LIKE SONG
    // ==================================================

    void likeCurrentSong() {

        int songId = playlist.getCurrentSong();

        if (songId == -1) {
            cout << "\nNo song is currently selected.\n";
            return;
        }

        songs[songId].likeCount++;

        cout << "\nYou liked: "
             << songs[songId].title
             << "\n";
    }

    // ==================================================
    //                    SKIP SONG
    // ==================================================

    void skipCurrentSong() {

        int songId = playlist.getCurrentSong();

        if (songId == -1) {
            cout << "\nNo song is currently selected.\n";
            return;
        }

        songs[songId].skipCount++;

        cout << "\nSkipped: "
             << songs[songId].title
             << "\n";

        nextSong();
    }

    // ==================================================
    //               DISPLAY PLAYLIST
    // ==================================================

    void showPlaylist() {
        playlist.display(songs);
    }

    // ==================================================
    //              SHOW RECENTLY PLAYED
    // ==================================================

    void showRecentlyPlayed() {

        if (recentlyPlayed.empty()) {
            cout << "\nNo recently played songs.\n";
            return;
        }

        stack<int> temp = recentlyPlayed;

        cout << "\n========== RECENTLY PLAYED ==========\n";

        int number = 1;

        while (!temp.empty()) {

            int songId = temp.top();

            cout << number << ". "
                 << songs[songId].title
                 << " - "
                 << songs[songId].artist
                 << "\n";

            temp.pop();
            number++;
        }
    }

    // ==================================================
    //               SHOW UPCOMING QUEUE
    // ==================================================

    void showUpcomingSongs() {

        if (playQueue.empty()) {
            cout << "\nNo songs in queue.\n";
            return;
        }

        queue<int> temp = playQueue;

        cout << "\n========== UPCOMING SONGS ==========\n";

        int number = 1;

        while (!temp.empty()) {

            int songId = temp.front();

            cout << number << ". "
                 << songs[songId].title
                 << " - "
                 << songs[songId].artist
                 << "\n";

            temp.pop();
            number++;
        }
    }

    // ==================================================
    //              SHOW TOP ARTISTS
    // ==================================================

    void showFavoriteArtists() {

        if (artistFrequency.empty()) {
            cout << "\nNo listening history available yet.\n";
            return;
        }

        priority_queue<pair<int, string>> topArtists;

        for (auto& pair : artistFrequency) {

            topArtists.push({
                pair.second,
                pair.first
            });
        }

        cout << "\n========== YOUR TOP ARTISTS ==========\n";

        int rank = 1;

        while (
            !topArtists.empty() &&
            rank <= 5
        ) {

            cout << rank << ". "
                 << topArtists.top().second
                 << " - "
                 << topArtists.top().first
                 << " plays\n";

            topArtists.pop();
            rank++;
        }
    }

    // ==================================================
    //                 SHOW STATISTICS
    // ==================================================

    void showStatistics() {

        cout << "\n========== MUSIC STATISTICS ==========\n";

        cout << "\nMood Preferences:\n";

        if (moodFrequency.empty()) {
            cout << "No mood data available.\n";
        }

        for (auto& pair : moodFrequency) {

            cout << pair.first
                 << ": "
                 << pair.second
                 << "\n";
        }

        cout << "\nSong Statistics:\n";

        for (auto& pair : songs) {

            Song& song = pair.second;

            if (
                song.playCount > 0 ||
                song.likeCount > 0 ||
                song.skipCount > 0
            ) {

                cout << "\n"
                     << song.title
                     << " - "
                     << song.artist
                     << "\n";

                cout << "  Plays: " << song.playCount << "\n";
                cout << "  Likes: " << song.likeCount << "\n";
                cout << "  Skips: " << song.skipCount << "\n";
            }
        }
    }

    // ==================================================
    //                RELATED SONGS
    // ==================================================

    void showRelatedSongs() {

        int currentSongId = playlist.getCurrentSong();

        if (currentSongId == -1) {
            cout << "\nNo current song selected.\n";
            return;
        }

        Song& currentSong = songs[currentSongId];

        if (currentSong.relatedSongs.empty()) {
            cout << "\nNo related songs available.\n";
            return;
        }

        cout << "\n========== RELATED SONGS ==========\n";

        for (int relatedId : currentSong.relatedSongs) {

            if (songs.find(relatedId) != songs.end()) {

                Song& related = songs[relatedId];

                cout << related.title
                     << " - "
                     << related.artist
                     << " ["
                     << related.mood
                     << "]\n";
            }
        }
    }

    // ==================================================
    //                     MAIN MENU
    // ==================================================

    void run() {

        int choice;

        do {

            cout << "\n\n" << PURPLE << "========================================\n";
            cout << PINK << "        MOODIFY MUSIC SYSTEM\n";
            cout << PURPLE << "========================================\n" << RESET;

            cout << LAVENDER << "1. Browse All Songs\n";
            cout << "2. Search Song\n";
            cout << "3. Select Mood & Generate Playlist\n";
            cout << "4. Play Current Song\n";
            cout << "5. Show Current Playlist\n";
            cout << "6. Next Song\n";
            cout << "7. Previous Song\n";
            cout << "8. Like Current Song\n";
            cout << "9. Skip Current Song\n";
            cout << "10. Recently Played\n";
            cout << "11. Upcoming Songs Queue\n";
            cout << "12. Show Favorite Artists\n";
            cout << "13. Show Related Songs\n";
            cout << "14. Show Listening Statistics\n";
            cout << "0. Exit\n" << RESET;

            cout << PINK << "\nEnter your choice: " << RESET;
            cin >> choice;

            // Input validation
            if (cin.fail()) {

                cin.clear();

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                cout << "\nPlease enter a valid number.\n";

                continue;
            }

            switch (choice) {

                case 1:
                    displayAllSongs();
                    break;

                case 2:
                    searchSong();
                    break;

                case 3:
                    generateRecommendations();
                    break;

                case 4:
                    playCurrentSong();
                    break;

                case 5:
                    showPlaylist();
                    break;

                case 6:
                    nextSong();
                    break;

                case 7:
                    previousSong();
                    break;

                case 8:
                    likeCurrentSong();
                    break;

                case 9:
                    skipCurrentSong();
                    break;

                case 10:
                    showRecentlyPlayed();
                    break;

                case 11:
                    showUpcomingSongs();
                    break;

                case 12:
                    showFavoriteArtists();
                    break;

                case 13:
                    showRelatedSongs();
                    break;

                case 14:
                    showStatistics();
                    break;

                case 0:
                    cout << "\nThank you for using Moodify!\n";
                    break;

                default:
                    cout << "\nInvalid choice. Try again.\n";
            }

        } while (choice != 0);
    }
};

// ======================================================
//                       MAIN FUNCTION
// ======================================================

int main() {

    Moodify app;

    app.run();

    return 0;
}