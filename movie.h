#pragma once
#include <string>
#include <vector>
using namespace std;

struct Movie {
    string tconst;
    string title;
    int year;
    vector<string> genres;
    double rating;
    int numVotes;
};