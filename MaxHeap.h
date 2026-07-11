#pragma once
#include "Movie.h"
#include "btree.h"

#include <vector>
using namespace std;

class MaxHeap {
public:
    void insert(const Movie& movie);
    Movie extractMax();
    bool isEmpty() const;
    int size() const;

private:
    vector<Movie> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int index) const;
    int leftChild(int index) const;
    int rightChild(int index) const;
};


vector<MaxHeap> genreMaxHeaps(vector<string> genres, vector<Movie> movies);
