//
// Created by Sunay K on 7/7/2026.
//

#ifndef FILM_ON_A_WHIM_BTREE_H
#define FILM_ON_A_WHIM_BTREE_H

#include <string>
#include <vector>

#include "movie.h"
using namespace std;

struct Key {
    double ratings;
    int votes;
};
string toLowerCase(string text);

struct Node {
    bool isLeaf = false;
    vector<Key> keys;
    Node* parent = nullptr;

    vector<Node*> children;
    vector<Movie> values;
    Node* next = nullptr;
};

vector<Movie> listTenMovies(vector<Movie> movies);

class btree {
private:
    Node* root;
    int order;




public:
    btree(int order);
    void insert(Movie m);
    vector<Movie> searchRange(double minRating, double maxRating);
};

vector<btree> genreBPlusTrees(vector<string> genres, vector<Movie> movies);



















#endif //FILM_ON_A_WHIM_BTREE_H