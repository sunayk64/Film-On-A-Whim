#pragma once
#include "Movie.h"
#include <vector>
#include <string>
using namespace std;

vector<Movie> loadMoviesFromCSV(const string& filename);