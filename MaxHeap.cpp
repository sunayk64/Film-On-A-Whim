#include "MaxHeap.h"
#include <stdexcept>
#include <algorithm>

int MaxHeap::parent(int index) const {
    return (index - 1)/2;
}

int MaxHeap::leftChild(int index) const {
    return 2 * index + 1;
}

int MaxHeap::rightChild(int index) const {
    return 2 * index + 2;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0 && heap[parent(index)].rating < heap[index].rating) {
        swap(heap[parent(index)], heap[index]);
        index = parent(index);
    }
}

void MaxHeap::heapifyDown(int index) {
    int heapSize = static_cast<int>(heap.size());

    while (true) {
        int left = leftChild(index);
        int right = rightChild(index);
        int largest = index;

        if (left < heapSize && heap[left].rating > heap[largest].rating) {
            largest = left;
        }
        if (right < heapSize && heap[right].rating > heap[largest].rating) {
            largest = right;
        }
        if (largest == index) {
            break;
        }

        swap(heap[index], heap[largest]);
        index = largest;
    }
}

void MaxHeap::insert(const Movie& movie) {
    heap.push_back(movie);
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

Movie MaxHeap::extractMax() {
    if (heap.empty()) {
        throw runtime_error("Cannot extract from an empty heap");
    }

    Movie maxMovie = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    return maxMovie;
}

bool MaxHeap::isEmpty() const {
    return heap.empty();
}

int MaxHeap::size() const {
    return static_cast<int>(heap.size());
}