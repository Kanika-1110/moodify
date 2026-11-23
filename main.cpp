#include <string>
#include "HashingModule.h"
#include "GraphModule.h"
#include "Interface.h"

using namespace std;
int main() {

    HashingModule library;
    GraphModule graph;
    
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

    library.addSong(6, "Stay",
                    "The Kid LAROI", "Sad");

    library.addSong(7, "Believer",
                    "Imagine Dragons", "Energetic");

    library.addSong(8, "Until I Found You",
                    "Stephen Sanchez", "Romantic");

    int nextSongId = 9;
    auto addCatalogSong = [&](const string &title,

        const string &artist,
const string &mood) {
        library.addSong(nextSongId++, title, artist, mood);
    };

    addCatalogSong("Ilahi", "Arijit Singh", "Happy");
    addCatalogSong("Love You Zindagi", "Amit Trivedi, Jasleen Royal", "Happy");
    addCatalogSong("Safarnama", "Lucky Ali", "Happy");
    addCatalogSong("Kho Gaye Hum Kahan", "Jasleen Royal, Prateek Kuhad", "Happy");
    addCatalogSong("Heeriye", "Jasleen Royal, Arijit Singh", "Happy");
    addCatalogSong("Ghungroo", "Arijit Singh, Shilpa Rao", "Happy");
    addCatalogSong("Gallan Goodiyaan", "Yashita Sharma, others", "Happy");
    addCatalogSong("Ullu Ka Pattha", "Arijit Singh, Nikhita Gandhi", "Happy");
    addCatalogSong("Aankhon Se Batana", "Dikshant", "Happy");
    addCatalogSong("Alag Aasmaan", "Anuv Jain", "Happy, Chill");
    addCatalogSong("Kasoor", "Prateek Kuhad", "Happy, Chill");
    addCatalogSong("Sunflower", "Post Malone, Swae Lee", "Happy");
    addCatalogSong("Golden", "Harry Styles", "Happy");
    addCatalogSong("Sunday Best", "Surfaces", "Happy");
    addCatalogSong("Good Life", "OneRepublic", "Happy");
    addCatalogSong("Best Day of My Life", "American Authors", "Happy");
    addCatalogSong("Birds of a Feather", "Billie Eilish", "Happy");
    addCatalogSong("Sweet Disposition", "The Temper Trap", "Happy");
    addCatalogSong("Walking on a Dream", "Empire of the Sun", "Happy");
    addCatalogSong("Beautiful Things", "Benson Boone", "Happy");

    addCatalogSong("Raat Ki Rani", "Seedhe Maut", "Romantic");
    addCatalogSong("Ranjha", "B Praak, Jasleen Royal", "Romantic");
    addCatalogSong("Tum Hi Ho", "Arijit Singh", "Romantic");
    addCatalogSong("Kesariya", "Arijit Singh", "Romantic");
    addCatalogSong("Apna Bana Le", "Arijit Singh", "Romantic");
    addCatalogSong("Hawayein", "Arijit Singh", "Romantic");
    addCatalogSong("Phir Le Aya Dil", "Arijit Singh", "Romantic");
    addCatalogSong("Agar Tum Saath Ho", "Alka Yagnik, Arijit Singh", "Romantic, Melancholy");
    addCatalogSong("Samjhawan", "Arijit Singh, Shreya Ghoshal", "Romantic");
    addCatalogSong("Teri Ore", "Rahat Fateh Ali Khan, Shreya Ghoshal", "Romantic");
    addCatalogSong("Saibo", "Shreya Ghoshal, Tochi Raina", "Romantic");
    addCatalogSong("Sun Raha Hai (Female Version)", "Shreya Ghoshal", "Romantic");
    addCatalogSong("Raabta", "Arijit Singh", "Romantic");
    addCatalogSong("Iktara", "Kavita Seth", "Romantic");
    addCatalogSong("Lover", "Taylor Swift", "Romantic");
    addCatalogSong("Those Eyes", "New West", "Romantic");
    addCatalogSong("Sweet", "Cigarettes After Sex", "Romantic");
    addCatalogSong("Yellow", "Coldplay", "Romantic");
    addCatalogSong("Line Without a Hook", "Ricky Montgomery", "Romantic");

    addCatalogSong("Roshni", "Seedhe Maut", "Chill");
    addCatalogSong("Do Guna", "Seedhe Maut", "Chill");
    addCatalogSong("Yaad", "Seedhe Maut", "Chill");
    addCatalogSong("Luka Chippi", "Seedhe Maut", "Chill");
    addCatalogSong("Khoj", "Seedhe Maut", "Chill");
    addCatalogSong("Husn", "Anuv Jain", "Chill");
    addCatalogSong("Baarishein", "Anuv Jain", "Chill");
    addCatalogSong("Gul", "Anuv Jain", "Chill");
    addCatalogSong("cold/mess", "Prateek Kuhad", "Chill");
    addCatalogSong("Tune Kaha", "Prateek Kuhad", "Chill");
    addCatalogSong("Aaoge Tum Kabhi", "The Local Train", "Chill");
    addCatalogSong("Choo Lo", "The Local Train", "Chill");
    addCatalogSong("Ocean Eyes", "Billie Eilish", "Chill");
    addCatalogSong("The Night We Met", "Lord Huron", "Chill, Melancholy");
    addCatalogSong("Apocalypse", "Cigarettes After Sex", "Chill");
    addCatalogSong("Nothing's Gonna Hurt You Baby", "Cigarettes After Sex", "Chill");
    addCatalogSong("Sweater Weather", "The Neighbourhood", "Chill");
    addCatalogSong("The Less I Know the Better", "Tame Impala", "Chill");

    addCatalogSong("A Walk", "Tycho", "Focused");
    addCatalogSong("Awake", "Tycho", "Focused");
    addCatalogSong("Weightless", "Marconi Union", "Focused");
    addCatalogSong("Experience", "Ludovico Einaudi", "Focused");
    addCatalogSong("Nuvole Bianche", "Ludovico Einaudi", "Focused");
    addCatalogSong("Comptine d'un autre été", "Yann Tiersen", "Focused");
    addCatalogSong("Intro", "The xx", "Focused");
    addCatalogSong("Sunset Lover", "Petit Biscuit", "Focused");
    addCatalogSong("Kerala", "Bonobo", "Focused");
    addCatalogSong("Cirrus", "Bonobo", "Focused");
    addCatalogSong("Open Eye Signal", "Jon Hopkins", "Focused");
    addCatalogSong("A Moment Apart", "ODESZA", "Focused");
    addCatalogSong("Kusanagi", "ODESZA", "Focused");
    addCatalogSong("Your Hand in Mine", "Explosions in the Sky", "Focused");
    addCatalogSong("First Breath After Coma", "Explosions in the Sky", "Focused");
    addCatalogSong("Time", "Hans Zimmer", "Focused");
    addCatalogSong("Cornfield Chase", "Hans Zimmer", "Focused");
    addCatalogSong("Snowfall", "Øneheart, reidenshi", "Focused");
    addCatalogSong("Resonance", "HOME", "Focused");
    addCatalogSong("Sunset", "The xx", "Focused");

    addCatalogSong("Kaanch Ke Ghar", "Seedhe Maut", "Melancholy");
    addCatalogSong("Maina", "Seedhe Maut", "Melancholy");
    addCatalogSong("Marne Ke Baad Bhi...", "Seedhe Maut", "Melancholy");
    addCatalogSong("Kohra", "Seedhe Maut", "Melancholy");
    addCatalogSong("Toh Kya", "Seedhe Maut", "Melancholy");
    addCatalogSong("Channa Mereya", "Arijit Singh", "Melancholy");
    addCatalogSong("Phir Bhi Tumko Chaahunga", "Arijit Singh", "Melancholy");
    addCatalogSong("Hamari Adhuri Kahani", "Arijit Singh", "Melancholy");
    addCatalogSong("Tadap Tadap", "KK", "Melancholy");
    addCatalogSong("Tujhe Bhula Diya", "Mohit Chauhan, Shekhar Ravjiani", "Melancholy");
    addCatalogSong("Aahista", "Arijit Singh, Jonita Gandhi", "Melancholy");
    addCatalogSong("Main Rahoon Ya Na Rahoon", "Armaan Malik", "Melancholy");
    addCatalogSong("Another Love", "Tom Odell", "Melancholy");
    addCatalogSong("Jealous", "Labrinth", "Melancholy");
    addCatalogSong("The 1", "Taylor Swift", "Melancholy");
    addCatalogSong("I Love You So", "The Walters", "Melancholy");
    addCatalogSong("Let Her Go", "Passenger", "Melancholy");
    addCatalogSong("Liability", "Lorde", "Melancholy");

    addCatalogSong("Nanchaku", "Seedhe Maut, MC STAN", "Energetic");
    addCatalogSong("Namastute", "Seedhe Maut", "Energetic");
    addCatalogSong("Khatta Flow", "Seedhe Maut, KR$NA", "Energetic");
    addCatalogSong("11K", "Seedhe Maut", "Energetic");
    addCatalogSong("Hola Amigo", "KR$NA, Seedhe Maut, Umair", "Energetic");
    addCatalogSong("SHUTDOWN", "Seedhe Maut", "Energetic");
    addCatalogSong("TT", "Seedhe Maut", "Energetic");
    addCatalogSong("Shaktimaan", "Seedhe Maut", "Energetic");
    addCatalogSong("Kranti", "Seedhe Maut", "Energetic");
    addCatalogSong("No Enema", "Seedhe Maut, Foreign Beggars", "Energetic");
    addCatalogSong("Bhussi", "KSHMR, Seedhe Maut, Karan Kanchan", "Energetic");
    addCatalogSong("Nalla Freestyle", "Seedhe Maut", "Energetic");
    addCatalogSong("Jashn-E-Bahaaraa", "Javed Ali", "Energetic");
    addCatalogSong("Zinda", "Siddharth Mahadevan", "Energetic");
    addCatalogSong("Malhari", "Vishal Dadlani", "Energetic");
    addCatalogSong("Industry Baby", "Lil Nas X, Jack Harlow", "Energetic");
    addCatalogSong("Starboy", "The Weeknd, Daft Punk", "Energetic");
    addCatalogSong("FE!N", "Travis Scott, Playboi Carti", "Energetic");
    addCatalogSong("Can't Hold Us", "Macklemore, Ryan Lewis", "Energetic");

    addCatalogSong("Anaadi", "Seedhe Maut", "Chill");
    addCatalogSong("Hoshiyaar", "Seedhe Maut", "Focused");
    addCatalogSong("Chidiya Udd", "Seedhe Maut", "Melancholy");
    addCatalogSong("Dum Ghutte", "Seedhe Maut", "Melancholy");
    addCatalogSong("Batti", "Seedhe Maut", "Energetic");
    addCatalogSong("101", "Seedhe Maut", "Energetic");
    addCatalogSong("Hausla", "Seedhe Maut", "Happy");
    addCatalogSong("Taakat", "Seedhe Maut", "Energetic");
    addCatalogSong("Bazaar (feat. Seedhe Maut)", "Seedhe Maut", "Chill");
    addCatalogSong("BUZZ (with KR$NA)", "Seedhe Maut, KR$NA", "Energetic");

    graph.connectSongs(1, 2);
    graph.connectSongs(1, 7);
    graph.connectSongs(2, 3);
    graph.connectSongs(3, 4);
    graph.connectSongs(4, 8);
    graph.connectSongs(5, 6);
    graph.connectSongs(6, 4);

    graph.connectSongs(3, 9);
    graph.connectSongs(9, 10);
    graph.connectSongs(10, 11);
    graph.connectSongs(11, 12);
    graph.connectSongs(12, 13);
    graph.connectSongs(13, 14);
    graph.connectSongs(14, 15);
    graph.connectSongs(15, 16);
    graph.connectSongs(16, 17);
    graph.connectSongs(17, 18);
    graph.connectSongs(18, 19);
    graph.connectSongs(19, 20);
    graph.connectSongs(20, 21);
    graph.connectSongs(21, 22);
    graph.connectSongs(22, 23);
    graph.connectSongs(23, 24);
    graph.connectSongs(24, 25);
    graph.connectSongs(25, 26);
    graph.connectSongs(26, 27);
    graph.connectSongs(27, 28);
    graph.connectSongs(28, 129);

    graph.connectSongs(4, 29);
    graph.connectSongs(29, 30);
    graph.connectSongs(30, 31);
    graph.connectSongs(31, 32);
    graph.connectSongs(32, 33);
    graph.connectSongs(33, 34);
    graph.connectSongs(34, 35);
    graph.connectSongs(35, 36);
    graph.connectSongs(36, 37);
    graph.connectSongs(37, 38);
    graph.connectSongs(38, 39);
    graph.connectSongs(39, 40);
    graph.connectSongs(40, 41);
    graph.connectSongs(41, 42);
    graph.connectSongs(42, 43);
    graph.connectSongs(43, 44);
    graph.connectSongs(44, 45);
    graph.connectSongs(45, 46);
    graph.connectSongs(46, 47);
    graph.connectSongs(47, 8);

    graph.connectSongs(18, 19);
    graph.connectSongs(19, 48);
    graph.connectSongs(48, 49);
    graph.connectSongs(49, 50);
    graph.connectSongs(50, 51);
    graph.connectSongs(51, 52);
    graph.connectSongs(52, 53);
    graph.connectSongs(53, 54);
    graph.connectSongs(54, 55);
    graph.connectSongs(55, 56);
    graph.connectSongs(56, 57);
    graph.connectSongs(57, 58);
    graph.connectSongs(58, 59);
    graph.connectSongs(59, 60);
    graph.connectSongs(60, 61);
    graph.connectSongs(61, 62);
    graph.connectSongs(62, 63);
    graph.connectSongs(63, 64);
    graph.connectSongs(64, 65);
    graph.connectSongs(65, 123);
    graph.connectSongs(123, 131);

    graph.connectSongs(66, 67);
    graph.connectSongs(67, 68);
    graph.connectSongs(68, 69);
    graph.connectSongs(69, 70);
    graph.connectSongs(70, 71);
    graph.connectSongs(71, 72);
    graph.connectSongs(72, 73);
    graph.connectSongs(73, 74);
    graph.connectSongs(74, 75);
    graph.connectSongs(75, 76);
    graph.connectSongs(76, 77);
    graph.connectSongs(77, 78);
    graph.connectSongs(78, 79);
    graph.connectSongs(79, 80);
    graph.connectSongs(80, 81);
    graph.connectSongs(81, 82);
    graph.connectSongs(82, 83);
    graph.connectSongs(83, 84);
    graph.connectSongs(84, 85);
    graph.connectSongs(85, 124);

    graph.connectSongs(36, 61);
    graph.connectSongs(61, 86);
    graph.connectSongs(86, 87);
    graph.connectSongs(87, 88);
    graph.connectSongs(88, 89);
    graph.connectSongs(89, 90);
    graph.connectSongs(90, 91);
    graph.connectSongs(91, 92);
    graph.connectSongs(92, 93);
    graph.connectSongs(93, 94);
    graph.connectSongs(94, 95);
    graph.connectSongs(95, 96);
    graph.connectSongs(96, 97);
    graph.connectSongs(97, 98);
    graph.connectSongs(98, 99);
    graph.connectSongs(99, 100);
    graph.connectSongs(100, 101);
    graph.connectSongs(101, 102);
    graph.connectSongs(102, 103);
    graph.connectSongs(103, 125);
    graph.connectSongs(125, 126);

    graph.connectSongs(1, 104);
    graph.connectSongs(104, 105);
    graph.connectSongs(105, 106);
    graph.connectSongs(106, 107);
    graph.connectSongs(107, 108);
    graph.connectSongs(108, 109);
    graph.connectSongs(109, 110);
    graph.connectSongs(110, 111);
    graph.connectSongs(111, 112);
    graph.connectSongs(112, 113);
    graph.connectSongs(113, 114);
    graph.connectSongs(114, 115);
    graph.connectSongs(115, 116);
    graph.connectSongs(116, 117);
    graph.connectSongs(117, 118);
    graph.connectSongs(118, 119);
    graph.connectSongs(119, 120);
    graph.connectSongs(120, 121);
    graph.connectSongs(121, 122);
    graph.connectSongs(122, 127);
    graph.connectSongs(127, 128);
    graph.connectSongs(128, 130);
    graph.connectSongs(130, 132);

    Interface app(library, graph);

    app.run();

    return 0;
}
