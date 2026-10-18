#include "Interface.h"
#include <iostream>
#include <limits>

using namespace std;

Interface::Interface(HashingModule &lib)
    : library(lib), recommendation(lib) {}

// Login or guest session
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

        // Demo credentials from the original project
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
    cout << "4. Exit Moodify\n";
    cout << "Enter your choice: ";
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
                cout << "Thanks for listening with Moodify, "
                     << username << "!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 4);
}