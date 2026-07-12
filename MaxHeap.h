#pragma once
#include "Movie.h"
#include <vector>
#include <string>
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


vector<MaxHeap> genreMaxHeaps(
    const vector<string>& genres,
    const vector<Movie>& movies
);