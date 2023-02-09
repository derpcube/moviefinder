/*
    2/2/2023

    The program creates a simple movie database and will populate the database by reading information from a file whose name will be entered by the user.
    Once the “database” has been populated appropriately, the user will be continually prompted to enter a movie title to search for.
    When the user enters a movie title, the program searches through the database for the movie title.
    If it is found, the movie’s information will be displayed; if it is not, an appropriate error message will be displayed.
    After the movie’s information has been displayed (assuming the movie has been found), the program asks the user if they would like to save it.
    If they chose to do so, the movie is added to a new, separate file called “favorites.txt”.
    Movies saved to the favorites file have the same format as they do in the original file.
    All saved movies from the favorites.txt file will be displayed when the user chooses to exit the program.
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct Movie {
    string title;
    double grossTotal;
    string director;
    string releaseDate;
    int runTime;
};

int numberOfLines(ifstream&);
void populateMovieFromFile(ifstream&, Movie&);
void displayMovie(const Movie&);
Movie* createDatabase(int&);
bool caseInsensitiveCmp(string, string);
void findMovie(Movie*, int);
void saveToFile(const Movie&);
bool promptToContinue();
void displayFavorites();
bool checkInput(string);
bool checkLetter(string);

int main() {
    int numberOfMovies;
    Movie* movieDatabase = createDatabase(numberOfMovies);

    while (true) {
        findMovie(movieDatabase, numberOfMovies);
        if (!promptToContinue()) {
            break;
        }
    }

    displayFavorites();

    delete[] movieDatabase;
    return 0;
}

/*
This function accepts an input file and returns an integer representing the number of
lines in the file
*/

int numberOfLines(ifstream& infile) {
    int count = 0;
    string line;

    while (getline(infile, line)) {
        count++;
    }

    return count;
}

/*
This function accepts an input file and a Movie object. Inside the function, a single line
is read in from the current read position of the input file. Once the line has been
read in, it will be parsed and stored in the Movie object that was passed to the function.
*/

void populateMovieFromFile(ifstream& inputFile, Movie& movie) {
    getline(inputFile, movie.title, ',');
    inputFile >> movie.grossTotal;
    inputFile.ignore(numeric_limits < streamsize > ::max(), ',');
    getline(inputFile, movie.director, ',');
    getline(inputFile, movie.releaseDate, ',');
    inputFile >> movie.runTime;
    inputFile.ignore(numeric_limits < streamsize > ::max(), '\n');
}

/*
This function accepts a Movie object passed by constant reference and displays the
contents of that object in a formatted fashion
*/

void displayMovie(const Movie& movie) {
    cout << "        Title: " << movie.title << endl;
    cout << "  Gross Total: " << movie.grossTotal << " billion dollars\n";
    cout << "     Director:" << movie.director << endl;
    cout << " Release Date:" << movie.releaseDate << endl;
    cout << "      Runtime: " << movie.runTime << " minutes" << endl;
}

/*
As the name suggests, this function is responsible for creating the movie database
(which is just a dynamically-allocated array of Movie objects). The integer argument
passed by reference to the function represents the number of movies in the database. It
is not initialized when the function is called, meaning its value will be set inside the
body of the function. This function prompts the user to enter an input file name.
If the name is invalid, displays an error and continue to prompt them until a valid file name has been entered.
The function:3
Determines the number of lines in the file using the numberOfLines function and creates your dynamically-allocated array of Movie objects
Loops through the newly-created array and stores a movie in each index. This is
where you the populateMovieFromFile function is called.
Populates the reference argument and return a pointer to the array of Movie objects
*/

Movie* createDatabase(int& totalMovies) {
    string fileName;

    cout << "Enter the name of the text file: ";
    getline(cin, fileName);

    cout << endl;

    ifstream file;
    file.open(fileName);

    while (!file.is_open()) {
        cout << "Error opening file" << endl;

        cout << "Enter the name of the text file: ";
        getline(cin, fileName);

        file.open(fileName);

        cout << endl;
    }

    totalMovies = numberOfLines(file);

    file.clear();
    file.seekg(0, ios::beg);

    Movie* movieDb = new Movie[totalMovies];

    for (int i = 0; i < totalMovies; i++) {
        populateMovieFromFile(file, movieDb[i]);
    }

    file.close();
    return movieDb;
}

/*
This function accepts two strings and performs a noncase-sensitive comparison on the
two. If the two strings are the same, the function returns true; otherwise, it returns false.
*/

bool caseInsensitiveCmp(string a, string b) {
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);
    return a < b;
}

/*
This function accepts a pointer to an array of Movie objects, and the
number of Movies (which is the same thing as the number of elements in the array). The
function:
Prompts the user to enter a movie title to search for
Searches through the array of Movie objects for the movie title
If the movie is found, displays the movie and ask the user if they would
like to save the movie; if they choose yes, the movie will be saved to
favorites.txt
If the movie was not found, displays an error message
*/

void findMovie(Movie* movies, int numMovies) {
    string targetTitle;
    string response;

    bool found = false;

    cout << "Enter a movie title to search for: ";
    getline(cin, targetTitle);

    transform(targetTitle.begin(), targetTitle.end(),
        targetTitle.begin(), ::tolower);

    for (int i = 0; i < numMovies; i++) {
        found = false;
        string title = movies[i].title;
        transform(title.begin(), title.end(),
            title.begin(), ::tolower);

        if (title == targetTitle) {
            found = true;

            cout << endl;

            displayMovie(movies[i]);

            cout << "\nWould you like to save the above movie? (Y or N): ";
            cin >> response;
            
            if (checkInput(response)) {

                if (response[0] == 'y' || response[0] == 'Y') {
                    saveToFile(movies[i]);
                }

                return;
            }
        }
    }
    cout << "\n   Error: " << targetTitle 
         << " not found in the database.Please try again..\n";
}

/*
This function accepts a Movie object, passed by constant reference. It saves the
Movie object to the “favorites.txt” file. If the file does not exist, the function creates it. If the file does
exist, the function the Movie object to the end of the file
*/

void saveToFile(const Movie& movie) {
    static bool t = true;

    ofstream outFile;

    if (t) {
        outFile.open("favorites.txt");
        t = false;
     }
    else {
        outFile.open("favorites.txt", ios::app);
    }

    if (outFile.is_open()) {
        outFile << movie.title << "," << movie.grossTotal << ","
                << movie.director << "," << movie.releaseDate
                << "," << movie.runTime << endl;

        outFile.close();

        cout << "Successfully saved to favorites.txt!\n";
    }
    else {
        cout << "Error opening file" << endl;
    }
}

/*
This function determines whether or not the user wants to continue using the
program. Firstly, it asks the user if they would like to exit the program and prompts them to
either enter ‘Y’ or ‘N’. If ‘Y’ or ‘y’ is entered, the function returns false (the user no
longer wishes to continue); otherwise, return true
*/

bool promptToContinue() {
    string response;
    bool trueInput = false;

    cout << "\nWould you like to exit? (Y/N): ";
    cin >> response;

    if (checkInput(response)) {
        cout << endl;

        cin.ignore();

        return response[0] == 'n' || response[0] == 'N';
    }
}

/*
This function displays all saved favorite movies by displaying all the information in the
favorites.txt file. If there are no saved movies, the function displays an error; otherwise, it displays
all saved movies.
*/

void displayFavorites() {
    ifstream inFile;
    inFile.open("favorites.txt");

    if (!inFile) {
        cout << "Unable to open file." << endl;
        return;
    }

    int count = 0;
    Movie favorites[100];

    while (!inFile.eof()) {
        populateMovieFromFile(inFile, favorites[count]);
        count++;
    }

    cout << "\nYour displayed movies are:\n";

    for (int i = 0; i < count - 1; i++) {
        displayMovie(favorites[i]);

        cout << endl;
    }

    inFile.close();
}

/*
This function validates the letter, making sure that the input is either Y, y, n, N.
*/

bool checkLetter(string input) {
    if (input[0] == 'y' || input[0] == 'Y'
        || input[0] == 'n' || input[0] == 'N') {
        return true;
    }
    return false;
}

/*
This function uses checkLetter to validate the whole input: make sure that it is a single character 
and either Y, y, n, N.
*/

bool checkInput(string response) {
    if (!checkLetter(response)) {
        while (!checkLetter(response)) {
            cout << "Please enter Y, y or N, n: ";
            cin >> response;

            if (response.size() > 1) {
                while (response.size() > 1) {
                    cout << "Please enter a single character: ";
                    cin >> response;
                }
            }
        }
    }
    return true;
}

/*
Enter the name of the text file: movies.txt

Enter a movie title to search for: AvaTar

        Title: Avatar
  Gross Total: 2.788 billion dollars
     Director: James Cameron
 Release Date: 12/18/09
      Runtime: 161 minutes

Would you like to save the above movie? (Y or N): y
Successfully saved to favorites.txt!

Would you like to exit? (Y/N): n

Enter a movie title to search for: FINDING dory

        Title: Finding Dory
  Gross Total: 1.029 billion dollars
     Director: Andrew Stanton
 Release Date: 6/17/16
      Runtime: 97 minutes

Would you like to save the above movie? (Y or N): Y
Successfully saved to favorites.txt!

Would you like to exit? (Y/N): N

Enter a movie title to search for: jurassic word

        Error: 'jurassic word' not found in the database. Please try again.

Would you like to exit? (Y/N): y


Your displayed movies are:
        Title: Avatar
  Gross Total: 2.788 billion dollars
     Director: James Cameron
 Release Date: 12/18/09
      Runtime: 161 minutes

        Title: Finding Dory
  Gross Total: 1.029 billion dollars
     Director: Andrew Stanton
 Release Date: 6/17/16
      Runtime: 97 minutes
*/