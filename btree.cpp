//
// Created by Sunay K on 7/7/2026.
//@ -0,0 +1,230 @@

#include <string>
#include <vector>

#include "Movie.h"
#include "btree.h"
#include <algorithm>
using namespace std;




btree::btree(int order_) {
    order = order_;
    root = nullptr;

}

void recursiveSplit(Node* root, Node* &actualRoot, int order) {


    int mid = root->keys.size() / 2;
    Node* p = new Node();
    p->isLeaf = false;
    if (root->isLeaf)
        p->isLeaf = true;
    Node* temp = root->next;
    root->next = p;
    p->next = temp;

    Key tempKey;
    if (p->isLeaf) {
        for (int i = mid; i < root->keys.size(); i++) {
            p->keys.push_back(root->keys[i]);
            p->values.push_back(root->values[i]);
        }

        root->keys.erase(root->keys.begin() + mid, root->keys.end());
        root->values.erase(root->values.begin() + mid,root->values.end());
        tempKey = p->keys[0];
    }
    else {
        tempKey = root->keys[mid];

        for (int i = mid+1; i < root->keys.size(); i++) {
            p->keys.push_back(root->keys[i]);
        }
        for (int i = mid + 1; i < root->children.size(); i++) {
            p->children.push_back(root->children[i]);
            root->children[i]->parent = p;
        }

        root->keys.erase(root->keys.begin() + mid, root->keys.end());
        root->children.erase(root->children.begin() + mid+1,root->children.end());
    }
    if (root->parent == nullptr) {
        Node* newRoot = new Node();
        newRoot->isLeaf = false;

        newRoot->keys.push_back(tempKey);
        newRoot->children.push_back(root);
        newRoot->children.push_back(p);

        root->parent = newRoot;
        p->parent = newRoot;
        actualRoot = newRoot;
        return;
    }

    for (int i = 0; i < root->parent->keys.size(); i++) {
        if (tempKey.ratings < root->parent->keys[i].ratings) {
            root->parent->keys.insert(root->parent->keys.begin() + i, tempKey);
            root->parent->children.insert(root->parent->children.begin() + i+1, p);
            p->parent = root->parent;
            if (root->parent->keys.size() >= order - 1) {
                recursiveSplit(root->parent, actualRoot, order);
            }
            return;
        }
    }
    root->parent->keys.insert(root->parent->keys.end(), tempKey);
    root->parent->children.insert(root->parent->children.end(), p);
    p->parent = root->parent;
    if (root->parent->keys.size() >= order - 1) {
        recursiveSplit(root->parent, actualRoot, order);
    }
    return;

}

void insertRecursive(Node* root, Movie m, int order,Node* &actualRoot) {
    if (root->isLeaf) {

        for (int i = 0; i < root->keys.size(); i++) {

            if (root->keys[i].ratings >= m.rating) {

                root->keys.insert(root->keys.begin() + i, {m.rating, m.numVotes});
                root->values.insert(root->values.begin() + i, m);

                if (root->keys.size() >= order-1) {
                    recursiveSplit(root, actualRoot, order);
                }
                return;
            }
        }
        root->keys.push_back({m.rating, m.numVotes});
        root->values.push_back(m);
        if (root->keys.size() >= order-1) {
            recursiveSplit(root, actualRoot, order);
        }
        return;
    }
    for (int i = 0; i < root->keys.size(); i++) {
        if (m.rating < root->keys[i].ratings) {
            insertRecursive(root->children[i], m, order,actualRoot);
            return;
        }
        if (m.rating == root->keys[i].ratings) {
            continue;
        }
    }
    insertRecursive(root->children[root->keys.size()], m, order, actualRoot);
}

void btree::insert(Movie m) {
    if (root == nullptr) {
        root = new Node();
        root->isLeaf = true;
        root->keys.push_back({m.rating, m.numVotes});
        root->values.push_back(m);
    }
    else {
        insertRecursive(root, m, order,root);
    }
}

Node* searchRecursive(Node* root, double minRating) {
    if (root->isLeaf) {
        return root;
    }
    for (int i = 0; i < root->keys.size(); i++) {
        if (root->keys[i].ratings >= minRating) {
            return searchRecursive(root->children[i], minRating);
        }
    }
    return searchRecursive(root->children.back(), minRating);
}


vector<Movie> btree::searchRange(double minRating, double maxRating) const {
    // null guard added: prevents crash when this genre's tree has no movies inserted
    if (root == nullptr) {
        return {};
    }
    Node* start = searchRecursive(root, minRating);

    bool found = false;
    vector<Movie> m;
    while (start != nullptr) {
        for (int i = 0; i < start->values.size(); i++) {
            if (start->values[i].rating < minRating) {
                continue;
            }
            if (start->values[i].rating > maxRating) {
                found = true;
                break;
            }
            m.push_back(start->values[i]);
        }
        if (found) {
            break;
        }
        start = start->next;
    }


    return m;
}

string toLowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}

vector<btree> genreBPlusTrees(
    const vector<string>& genres,
    const vector<Movie>& movies
) {
    vector<btree> bPlusTrees;
    for (size_t i = 0; i < genres.size(); i++) {
        btree n(32);
        bPlusTrees.push_back(n);
    }
    for (const Movie& m : movies) {
        for (const string& g : m.genres) {
            string lower = toLowerCase(g);
            for (size_t j = 0; j < genres.size(); j++) {
                if (genres[j] == lower) {
                    bPlusTrees[j].insert(m);
                    break;
                }
            }
        }
    }
    return bPlusTrees;
}