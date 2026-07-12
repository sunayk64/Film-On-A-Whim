#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "DataLoader.h"
#include "Movie.h"
#include "btree.h"
#include "MaxHeap.h"

using namespace std;

const int RESULT_COUNT = 5; // how many movies to actually show the user


vector<string> validGenres = {
    "action","adventure","animation","biography","comedy","crime",
    "documentary","drama","family","fantasy","film-noir","history",
    "horror","music","musical","mystery","news","romance","sci-fi",
    "sport","thriller","war","western"
};

void printHeader() {
    cout << "\n========================================\n";
    cout << "          *** Film on a Whim ***        \n";
    cout << "========================================\n";
}

void showAbout() {
    printHeader();
    cout << "\nFilm on a Whim helps users find random movie recommendations\n";
    cout << "based on a genre and IMDb rating range.\n\n";
    cout << "Matching movies are retrieved using both a custom Max Heap\n";
    cout << "and a B+ Tree. We then randomly select five results from\n";
    cout << "each structure for comparison.\n";
}

bool isValidGenre(string genre) {

    for (string validGenre : validGenres) {
        if (genre == validGenre) {
            return true;
        }
    }

    return false;
}

void findMovies(
    const vector<btree>& genreTrees,
    const vector<MaxHeap>& maxHeaps
){
    string input;
    string genre;
    double minRating;
    double maxRating;

    printHeader();

    cout << "\nAvailable genres:\n";
    cout << "action, adventure, animation, biography, comedy, crime,\n";
    cout << "documentary, drama, family, fantasy, film-noir, history,\n";
    cout << "horror, music, musical, mystery, news, romance, sci-fi,\n";
    cout << "sport, thriller, war, western\n";

    while (true) {
        cout << "\nEnter genre: ";
        getline(cin, input);
        input = toLowerCase(input);

        if (isValidGenre(input)) {
            genre = input;
            break;
        }

        cout << "Invalid genre. Try again.\n";
    }

    while (true) {
        cout << "Minimum rating: ";
        getline(cin, input);
        try {
            minRating = stod(input);
            break;
        } catch (...) {
            cout << "Please enter a valid number.\n";
        }
    }

    while (true) {
        cout << "Maximum rating: ";
        getline(cin, input);
        try {
            maxRating = stod(input);
            break;
        } catch (...) {
            cout << "Please enter a valid number.\n";
        }
    }

    cout << "\nSearching for " << genre << " movies rated ";
    cout << minRating << " to " << maxRating << "...\n\n";

    cout << "\n \n \n MaxHeap Search: \n" << endl;

    // Drain the heap into a rating-sorted vector covering the entire matched range.
    vector<Movie> pool;

    int genreIndex = -1;
    for (size_t gi = 0; gi < validGenres.size(); gi++) {
        if (validGenres[gi] == genre) {
            genreIndex = static_cast<int>(gi);
            break;
        }
    }
    if (genreIndex == -1 ||
        genreIndex >= static_cast<int>(maxHeaps.size()) ||
        genreIndex >= static_cast<int>(genreTrees.size())) {
        cout << "Genre data is unavailable.\n";
        return;
    }
    if (minRating > maxRating) {
        cout << "Minimum rating cannot be greater than maximum rating.\n";
        return;
    }

    MaxHeap heapCopy = maxHeaps[genreIndex];

    while (!heapCopy.isEmpty()) {
        Movie top = heapCopy.extractMax();
        if (top.rating > maxRating) {
            continue;
        }
        if (top.rating < minRating) {
            break;
        }
        pool.push_back(top);
    }

    // Randomly shuffle across the full range so selections aren't skewed toward the top.
    static mt19937 rng(random_device{}());
    shuffle(pool.begin(), pool.end(), rng);

    int showCount = min(RESULT_COUNT, static_cast<int>(pool.size()));

    for (int i = 0; i < showCount; i++) {
        cout << i + 1 << ". " << pool[i].title
             << " (" << pool[i].year << ")"
             << " | Rating: " << pool[i].rating
             << " | Votes: " << pool[i].numVotes << endl;
    }
    if (pool.empty()) {
        cout << "No movies found in that heap for this genre/range.\n";
    }

    cout << "\n\n\nBPlus Tree Search:\n" << endl;

    // Get every matching movie from the B+ Tree
    vector<Movie> treePool =
        genreTrees[genreIndex].searchRange(minRating, maxRating);

    // Apply the same randomization afterward
    shuffle(treePool.begin(), treePool.end(), rng);

    int treeShowCount =
        min(RESULT_COUNT, static_cast<int>(treePool.size()));

    for (int i = 0; i < treeShowCount; i++) {
    cout << i + 1 << ". " << treePool[i].title
         << " (" << treePool[i].year << ")"
         << " | Rating: " << treePool[i].rating
         << " | Votes: " << treePool[i].numVotes << endl;
    }

    if (treePool.empty()) {
        cout << "No movies found in that tree for this genre/range.\n";
    }


}

void mainMenu(
    const vector<btree>& genreTrees,
    const vector<MaxHeap>& maxHeaps
){
    string input;

    while (true) {
        printHeader();
        cout << "1. Find Movies\n";
        cout << "2. About\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";

        getline(cin, input);

        if (input == "1") {
            findMovies(genreTrees, maxHeaps);
        }
        else if (input == "2") {
            showAbout();
        }
        else if (input == "3") {
            cout << "\nThank you for using Film on a Whim. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
        }

        cout << "\nPress Enter to return to the main menu...";
        getline(cin, input);
    }
}

int main() {
    vector<Movie> movies = loadMoviesFromCSV("Movies.csv");
    vector<btree> genreTrees = genreBPlusTrees(validGenres,movies);
    vector<MaxHeap> maxHeaps = genreMaxHeaps(validGenres, movies);

    cout << "Loaded " << movies.size() << " movies successfully.\n";

    mainMenu(genreTrees, maxHeaps);

    return 0;
}