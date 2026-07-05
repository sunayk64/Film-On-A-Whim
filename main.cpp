#include <iostream>
#include <algorithm>
#include <vector>
#include "DataLoader.h"
#include "Movie.h"

using namespace std;

string toLowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}

void printHeader() {
    cout << "\n========================================\n";
    cout << "          *** Film on a Whim ***        \n";
    cout << "========================================\n";
}

void showAbout() {
    printHeader();
    cout << "\nFilm on a Whim helps users find random movie recommendations\n";
    cout << "based on a genre and IMDb rating range.\n\n";
    cout << "For the final project, we are comparing:\n";
    cout << "- Custom Max Heap\n";
    cout << "- Custom B+ Tree\n";
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
    string validGenres[23] = {
        "action","adventure","animation","biography","comedy","crime",
        "documentary","drama","family","fantasy","film-noir","history",
        "horror","music","musical","mystery","news","romance","sci-fi",
        "sport","thriller","war","western"
    };

    for (string validGenre : validGenres) {
        if (genre == validGenre) {
            return true;
        }
    }

    return false;
}

void findMovies(const vector<Movie>& movies) {
    string input;
    string genre;
    double minRating;
    double maxRating;

    printHeader();

    cout << "\nAvailable genres:\n";
    cout << "action, adventure, animation, biography, comedy, crime,\n";
    cout << "documentary, drama, family, fantasy, horror, mystery,\n";
    cout << "romance, sci-fi, sport, thriller, war, western\n";

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

    cout << "Minimum rating: ";
    getline(cin, input);
    minRating = stod(input);

    cout << "Maximum rating: ";
    getline(cin, input);
    maxRating = stod(input);

    cout << "\nSearching for " << genre << " movies rated ";
    cout << minRating << " to " << maxRating << "...\n\n";

    /*
        for integration:
        This is where the heap and B+ tree search results will go.
    */

    int count = 0;

    // Temporary linear search just to prove the menu and data work
    for (const Movie& movie : movies) {
        if (hasGenre(movie, genre) &&
            movie.rating >= minRating &&
            movie.rating <= maxRating) {

            cout << count + 1 << ". " << movie.title
                 << " (" << movie.year << ")"
                 << " | Rating: " << movie.rating
                 << " | Votes: " << movie.numVotes << endl;

            count++;

            if (count == 5) {
                break;
            }
        }
    }

    if (count == 0) {
        cout << "No matching movies found.\n";
    }
}

void mainMenu(const vector<Movie>& movies) {
    string input;

    while (true) {
        printHeader();
        cout << "1. Find Movies\n";
        cout << "2. About\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";

        getline(cin, input);

        if (input == "1") {
            findMovies(movies);
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

    cout << "Loaded " << movies.size() << " movies successfully.\n";

    mainMenu(movies);

    return 0;
}