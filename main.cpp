#include <iostream> 
#include <fstream>
#include <algorithm>
using namespace std; 

void Menu(bool isHeap) {
   string input;
    bool validGenre;
   string genres[23] = {"action","adventure","animation","biography","comedy","crime","documentary","drama","family",
                    "fantasy","film-noir","history","horror","music","musical","mystery","news","romance","sci-fi",
                    "sport","thriller","war","western",};
   cout << "Film On a Whim \n " << endl;
    for(string i:genres) {
        cout << i << ", ";
    }
    cout << "\n" << endl;
    while (true) {
        cout << " Please enter a genre of the movie you would like to watch:" << endl;
        getline(cin, input);
        validGenre = false;
        for (int i = 0; i < 23; i++) {
            if (input == genres[i])
                validGenre = true;
        }
        if (!validGenre) {
            cout << "Invalid Genre. Please enter again." << endl;
        }
        else {
            break;
        }
    }
    string genre = input;
    float rating;
    while (true) {
        cout << "Now please enter the minimum rating for the movie(0-10):" << endl;
        getline(cin, input);

        try {
            rating = stof(input);
            if (rating < 0.0f || rating > 10.0f)
                throw out_of_range("Rating must be between 0 and 10");
            break;
        }
        catch (invalid_argument& e) {
            cout << "Invalid input. Please enter a number.\n";
        }
        catch (out_of_range& e) {
            cout << e.what() << "\n";
        }
    }

    cout << "Searching for a " << genre << " movie with minimum rating of " << rating << "." << endl;
    // logic for searching, either heap or hashmap.

}



void mainMenu() {
    string input;
    cout << "enter the number for the correct options" << endl;
    cout << "Would you like to use a (1)Heap or a (2)HashMap?" << endl;
    getline(cin, input);
    if(input == "1")
        Menu(true);
    else if(input == "2")
        Menu(false);
    else
        cout << "invalid input, please enter a valid option" << endl;
        mainMenu();
}



int main() { 
    mainMenu();

    return 0; 
}