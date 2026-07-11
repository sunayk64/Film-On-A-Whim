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
    cout << "Matching movies are loaded into a custom Max Heap so we can\n";
    cout << "efficiently retrieve every result within your chosen genre\n";
    cout << "and rating range. We then randomly select five of them so\n";
    cout << "you get a actual random recommendation.\n";
}

bool hasGenre(const Movie& movie, string searchGenre) {
    for (string genre : movie.genres) {
        if (toLowerCase(genre) == searchGenre) {
            return true;
        }
    }
    return false;
}

bool isValidGenre(string genre) {

    for (string validGenre : validGenres) {
        if (genre == validGenre) {
            return true;
        }
    }

    return false;
}

void findMovies(const vector<Movie>& movies, vector<btree> genreTrees,vector<MaxHeap> maxHeaps) {
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
    for (int gi = 0; gi < 23; gi++) {
        if (validGenres[gi] == genre) {
            genreIndex = gi;
            break;
        }
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

    cout << "\n \n \n BPlus Tree Search: \n" << endl;

    // BPlus Tree search
    vector<Movie> topMovies;
    for (int i = 0; i < 23; i++) {
        if (validGenres[i] == genre) {
            topMovies = genreTrees[i].searchRange(minRating, maxRating);
        }
    }
    int i = 1;
    for (Movie movie: topMovies) {
        cout << i<< "." << movie.title << "(" << movie.year << ")" << " | Rating: " <<movie.rating << " | Votes: " << movie.numVotes <<  endl;
        i++;
        if (i > showCount)
            break;
    }

    if (topMovies.empty()) {
        cout << "No movies found in that tree for this genre/range.\n";
    }


}

void mainMenu(const vector<Movie>& movies, vector<btree> genreTrees, vector<MaxHeap> maxHeaps) {
    string input;

    while (true) {
        printHeader();
        cout << "1. Find Movies\n";
        cout << "2. About\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";

        getline(cin, input);

        if (input == "1") {
            findMovies(movies, genreTrees,maxHeaps);
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

    mainMenu(movies, genreTrees,maxHeaps);

    return 0;
}