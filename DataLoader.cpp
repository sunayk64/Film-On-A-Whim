#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

vector<string> parseLine(const string& line) {
    vector<string> fields;
    string current;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];

        // Handles commas inside of quoted fields. Example: "Action,Adventure"
        if (ch == '"') {
            if (insideQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                i++;
            } else {
                insideQuotes = !insideQuotes;
            }
        } else if (ch == ',' && !insideQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }

    fields.push_back(current);
    return fields;
}

vector<string> splitGenres(const string& genreString) {
    vector<string> genres;
    stringstream ss(genreString);
    string genre;

    while (getline(ss, genre, ',')) {
        genres.push_back(genre);
    }

    return genres;
}

vector<Movie> loadMoviesFromCSV(const string& filename) {
    vector<Movie> movies;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return movies;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        vector<string> fields = parseLine(line);

        if (fields.size() < 9) {
            continue;
        }

        string titleType = fields[2];
        string genres = fields[6];

        if (titleType != "movie" || genres == "\\N") {
            continue;
        }

        try {
            Movie movie;
            movie.tconst = fields[1];
            movie.title = fields[3];
            movie.year = stoi(fields[5]);
            movie.genres = splitGenres(genres);
            movie.rating = stod(fields[7]);
            movie.numVotes = stoi(fields[8]);

            movies.push_back(movie);
        }
        catch (...) {
            // If one row has missing or weird data, skip it instead of ending the program.
            continue;
        }
    }

    return movies;
}